from django.shortcuts import get_object_or_404, render, redirect
from viz.models import *
from cube.models import CubeUser
from comments.models import Comment
from django.http import HttpResponse
from django.http import JsonResponse
from django.utils.html import escape
from django.conf import settings
from django.views.decorators.csrf import csrf_exempt
from django.http import Http404  
import logging
import datetime
import subprocess
import os
from django.core.files import File
from itertools import chain
from django.db.models.query import QuerySet
log = logging.getLogger(__name__)

def filter(request):
    return render(request, "viz/filter.html")

def appInfo(request):
    publicApps=Viz.objects.all().exclude(published=False)
    code=[]
    appIds=[]
    for app in publicApps:
        appCode=SourceCode.objects.get(viz=app)
        code.append(appCode.code)
        appIds.append(app.pk)
    response={ "apps": appIds , 
               "code" : code }
    return JsonResponse(response)


def gallery(request, filter="newestFirst"):
    if(filter=='newestFirst'):
        vizs=Viz.objects.all().order_by("-created").exclude(published=False)
    else:
        vizs=Viz.objects.all().order_by("created").exclude(published=False)      

    totalObjects=vizs.count()
    if totalObjects<8:
        visualizations=vizs[:totalObjects]
    else:
        visualizations=vizs[:8]
    return render(request, "viz/gallery.html", { 'visualizations' : visualizations , 'nextPage' : 1, 'totalObjects' : totalObjects, 'filter': filter})

@csrf_exempt
def compile(request):
    log.debug("compiling")
    code=request.POST['code']
    vizName=request.POST['vizName']
    vizId=request.POST['vizId']
    if vizId==None:
        vizId=-1
    else:
        vizId=int(vizId)
    log.debug(vizName);
    if vizName==None:
        vizName="undefined"

    code="%s\nchar* vizName=\"%s\";\nint vizId=%d;\n%s" % (settings.SPARK_LIBRARY, vizName, vizId, code)


    lines=code.split('\n')
    i=0
    code=""
    setupStarted=False
    codeInserted=False
    for line in lines:
        if not codeInserted:
            if not setupStarted:
                if line.find("setup()")!=-1:
                    setupStarted=True
            else:
                if line.find("}")!=-1:
                    log.debug("inserting code")
                    code="%s\n%s" % (code,  "Spark.variable(\"vizName\", vizName, STRING);\nSpark.variable(\"vizId\", &vizId, INT);")                
                    codeInserted=True
        code="%s\n%s" % (code, line)
        i+=1

    '''
    lines=code.split('\n')
    i=0
    for line in lines:
    log.debug("%d:  %s" % ( i, line))
    i+=1
    '''
    timestamp=datetime.datetime.now().strftime('%Y-%m-%d--%H.%M.%S')
    filename=timestamp+".cpp"
    media_root="/home/glass/cubetube-production/media/"
    project_root="/home/glass/cubetube-production/"
    directory= media_root+"sparkware/" + settings.CUBE_LIBRARY + "/firmware/examples/"
    log.debug("compiling %s%s" % (directory, filename))
    f = open(directory + filename, 'w')
    log.debug(directory)
    log.debug(filename)
    codeFile=File(f)
    codeFile.write(code)
    codeFile.close()
    command = ['make', '-C', '%ssparkware/%s' % (media_root, settings.CUBE_LIBRARY), 'bin/%s.bin' % timestamp]
    log.debug(command)
#    p = subprocess.Popen(['make', '-C', '%ssparkware/%s' % (media_root, settings.CUBE_LIBRARY), 'bin/%s.bin' % timestamp], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    p = subprocess.Popen(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE)


    output=[]
    log.debug("compiling code")
    i=0
    for line in p.stdout.readlines():
        log.debug(i)
        log.debug(line)
        line.replace('"','\\"')
        line.replace("'","\\'")
        output.append(line)
        i+=1

    error=[]
    for line in p.stderr.readlines():
        log.debug(line)
        line.replace('"','\\"')
        line.replace("'","\\'")
        error.append(line)

    log.debug("waiting for compile to finish")
    retval = p.wait()
    log.debug("compiler's done, moving on")

    binaryPath= media_root+"sparkware/%s/bin/%s.bin" % (settings.CUBE_LIBRARY, timestamp)
    flash_output=[]
    flash_error=[]

    if os.path.isfile(binaryPath):
        accessToken=request.POST['accessToken']
        coreID=request.POST['coreID']        
        p = subprocess.Popen(['node', '%s/viz/utils/flash.js' % project_root ,accessToken, coreID, binaryPath], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        for line in p.stdout.readlines():
            print line,
            line.replace('"','\\"')
            line.replace("'","\\'")
            flash_output.append(line)
            
        for line in p.stderr.readlines():
            print line,
            line.replace('"','\\"')
            line.replace("'","\\'")
            flash_error.append(line)
            
        compilation_status="ok"
        retval = p.wait()
    else:
        compilation_status="failed"
    response={ "compilation_status": compilation_status , 
               "output" : output , 
               "error" : error , 
               "flash_output" : flash_output , 
               "flash_error" : flash_error}
    return JsonResponse(response)


@csrf_exempt
def cloudFlash(request):
    log.debug("compiling")
    code=request.POST['code']
    accessToken=request.POST['accessToken']


    vizName=request.POST['vizName']
    vizId=request.POST['vizId']
    if vizId==None:
        vizId=-1
    else:
        vizId=int(vizId)
    log.debug(vizName);
    if vizName==None:
        vizName="undefined"


    #not sure if this is causing a problem with connectivity, pulling it out for now
    #code="%s\nchar* vizName=\"%s\";\nint vizId=%d;\n%s" % (settings.SPARK_LIBRARY, vizName, vizId, code)
    code="%s\n%s" % (settings.SPARK_LIBRARY, code)

    lines=code.split('\n')
    i=0
    code=""
    for line in lines:
        code="%s\n%s" % (code, line)
        i+=1

    #original version, inserts viz ID and name as spark variables -- I suspect it's monkeying with stuff
    '''
    lines=code.split('\n')
    i=0
    code=""
    setupStarted=False
    codeInserted=False
    for line in lines:
        if not codeInserted:
            if not setupStarted:
                if line.find("setup()")!=-1:
                    setupStarted=True
            else:
                if line.find("}")!=-1:
                    log.debug("inserting code")
                    code="%s\n%s" % (code,  "Spark.variable(\"vizName\", vizName, STRING);\nSpark.variable(\"vizId\", &vizId, INT);")                
                    codeInserted=True
        code="%s\n%s" % (code, line)
        i+=1

    '''

    '''

    lines=code.split('\n')
    i=0
    code=""
    setupStarted=False
    codeInserted=False
    for line in lines:
        if not setupCodeInserted:
            if not setupStarted:
                if line.find("setup()")!=-1:
                    setupStarted=True
            else:
                if line.find("}")!=-1:
                    log.debug("inserting code")
                    code="%s\n%s" % (code,  "Spark.variable(\"vizName\", vizName, STRING);\nSpark.variable(\"vizId\", &vizId, INT);")                
                    setupCodeInserted=True
            if not setupCodeInserted:
            if not setupStarted:
                if line.find("setup()")!=-1:
                    setupStarted=True
            else:
                if line.find("}")!=-1:
                    log.debug("inserting code")
                    code="%s\n%s" % (code,  "Spark.variable(\"vizName\", vizName, STRING);\nSpark.variable(\"vizId\", &vizId, INT);")                
                    setupCodeInserted=True
        code="%s\n%s" % (code, line)
        i+=1
    '''

    timestamp=datetime.datetime.now().strftime('%Y-%m-%d--%H.%M.%S')
    filename=timestamp+".ino"
    directory=settings.CODE_DIRECTORY
    log.debug("saving code to file %s%s" % (directory, filename))
    f = open(directory + filename, 'w')
    log.debug(directory)
    log.debug(filename)
    codeFile=File(f)
    codeFile.write(code)
    codeFile.close()
    deviceID=request.POST['deviceID']        
    command=['node', 'cloudflash.js', '%s' % accessToken, '%s' % deviceID, '%s' % filename]
    log.debug(command)
    p = subprocess.Popen(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE, cwd=directory)
    jsonResult=""
    for line in p.stdout.readlines():
        jsonResult="%s%s" % (jsonResult, line)
        i+=1
    retval = p.wait() 
        
    return JsonResponse(jsonResult, safe=False)


@csrf_exempt
def justCompile(request):
    code=request.POST['code']
    code="%s\n%s" % (settings.SPARK_LIBRARY, code)
    lines=code.split('\n')
    i=0
    code=""
    setupStarted=False
    codeInserted=False
    for line in lines:
        if not codeInserted:
            if not setupStarted:
                if line.find("setup()")!=-1:
                    setupStarted=True
            if line.find("}")!=-1:
                log.debug("inserting code")
                codeInserted=True
        code="%s\n%s" % (code, line)
        i+=1

    '''
    lines=code.split('\n')
    i=0
    for line in lines:
    log.debug("%d:  %s" % ( i, line))
    i+=1
    '''
    timestamp=datetime.datetime.now().strftime('%Y-%m-%d--%H.%M.%S')
    filename=timestamp+".cpp"
    media_root="/home/glass/cubetube-production/media/"
    project_root="/home/glass/cubetube-production/"
    directory= media_root+"sparkware/" + settings.CUBE_LIBRARY + "/firmware/examples/"
    log.debug("compiling %s%s" % (directory, filename))
    f = open(directory + filename, 'w')
    log.debug(directory)
    log.debug(filename)
    codeFile=File(f)
    codeFile.write(code)
    codeFile.close()
    command = ['make', '-C', '%ssparkware/%s' % (media_root, settings.CUBE_LIBRARY), 'bin/%s.bin' % timestamp]
    log.debug(command)
    p = subprocess.Popen(['make', '-C', '%ssparkware/%s' % (media_root, settings.CUBE_LIBRARY), 'bin/%s.bin' % timestamp], stdout=subprocess.PIPE, stderr=subprocess.PIPE)


    output=[]
    for line in p.stdout.readlines():
        print line,
        line.replace('"','\\"')
        line.replace("'","\\'")
        output.append(line)

    error=[]
    for line in p.stderr.readlines():
        print line,
        line.replace('"','\\"')
        line.replace("'","\\'")
        error.append(line)

    retval = p.wait()

    binaryPath= media_root+"sparkware/%s/bin/%s.bin" % (settings.CUBE_LIBRARY, timestamp)

    if os.path.isfile(binaryPath):
        compilation_status="ok"
    else:
        compilation_status="failed"
    response={ "compilation_status": compilation_status , 
               "output" : output , 
               "error" : error , 
               }
    return JsonResponse(response)


def jsgallery(request, filter="newestFirst", featuredViz=None, vizCreator=None):
    accessToken=request.COOKIES.get('accessToken') 
    try:
        user=CubeUser.objects.filter(accessToken=accessToken).get()
    except CubeUser.DoesNotExist:
        user = None
    if user:
        userVizs=Viz.objects.filter(creator=user).order_by("-created")
        privateVizs=userVizs.filter(published=False).order_by("-created")
        publicVizs=userVizs.filter(published=True).order_by("-created")
    else:
        privateVizs=None
        publicVizs=None

    if(filter=='newestFirst'):
        vizs=Viz.objects.all().order_by("-pageViews", "-created").exclude(published=False)    
    elif(filter=='byCreator'):
        try:
            vizUser=CubeUser.objects.filter(nickname=vizCreator).first()
        except CubeUser.DoesNotExist:
            vizUser=None
        if(vizUser):        
            if(user):
                if(vizUser.nickname!=user.nickname):
                    vizs=Viz.objects.all().filter(creator=vizUser.id).order_by("-created").exclude(published=False)
                else:
                    vizs=Viz.objects.all().filter(creator=vizUser.id, published=True).order_by("-created")
            else:
                vizs=Viz.objects.all().filter(creator=vizUser.id).order_by("-created").exclude(published=False)
        else:
            vizs=None      
    else:
        vizs=Viz.objects.all().order_by("created").exclude(published=False)      
    
    if featuredViz is None:
        featured=None
    else:
        featured=Viz.objects.get(pk=featuredViz)

    if vizs is None:
        totalObjects=0
    else:
        totalObjects=vizs.count()
    
    if totalObjects==0:
        return render(request, "viz/jsgallery.html", { 'visualizations' : None , 'nextPage' : False, 'totalObjects' : totalObjects, 'filter': filter, 'featuredViz' : featured, 'privateVizs': privateVizs, 'publicVizs':publicVizs})        
    elif totalObjects<8:
        visualizations=vizs[:totalObjects]
        return render(request, "viz/jsgallery.html", { 'visualizations' : visualizations , 'nextPage' : False, 'totalObjects' : totalObjects, 'filter': filter, 'featuredViz' : featured, 'privateVizs': privateVizs, 'publicVizs':publicVizs})
    else:
        visualizations=vizs[:8]
        return render(request, "viz/jsgallery.html", { 'visualizations' : visualizations , 'nextPage' : 1, 'totalObjects' : totalObjects, 'filter': filter, 'featuredViz' : featured, 'privateVizs': privateVizs, 'publicVizs':publicVizs})

def index(request):
#    vizs=Viz.objects.all().order_by("-created").exclude(published=False)
    vizs=Viz.objects.all().order_by("-created").filter(featured=True).exclude(published=False)
    totalObjects=vizs.count()
    if totalObjects<8:
        visualizations=vizs[:totalObjects]
    else:
        visualizations=vizs[:8]
    return render(request, "viz/index.html", { 'visualizations' : visualizations, 'totalObjects' : totalObjects})

@csrf_exempt
def fork(request, vizId=None):
    accessToken = request.COOKIES['accessToken']
    log.debug("access token: %s" % accessToken)
    vizId=int(vizId)
    log.debug("viz ID: %d" % vizId)
    try:
        user=CubeUser.objects.filter(accessToken=accessToken).get()
    except CubeUser.DoesNotExist:
        user = None
    if user:
        viz=Viz.objects.get(pk=vizId)
        forked=Viz()
        forked.name=viz.name
        forked.description=viz.description
        forked.vizType=viz.vizType;
        forked.creator=user
        forked.parent=viz
        forked.save()
        source=SourceCode.objects.filter(viz=viz)
        for code in source:
            newCode=SourceCode()
            newCode.viz=forked
            newCode.code=code.code
            newCode.save()

        return redirect('edit', id=forked.pk)
    else:
        nickname    = request.COOKIES['nickname']
        accessToken = request.COOKIES['accessToken']
        if authenticate(nickname, accessToken):
            return render(request, "viz/authentication-error.html", 
                          { "nickname": nickname,
                            "accessToken": accessToken,
                            "authenticated":authenticate(nickname, accessToken)})
        else: raise Http404 

def viz(request, id):
    currentViz=Viz.objects.get(pk=id)
    try:
        binary=Binary.objects.get(viz=currentViz)
    except Binary.DoesNotExist:
        binary=None
    
    try:
        photo = Photo.objects.filter(viz=currentViz)[:1].get()  #get the main image associated with this viz, and use it as the photo
    except Photo.DoesNotExist:
        photo = False

    comments=Comment.objects.filter(viz=currentViz)
    currentViz.pageViews=currentViz.pageViews+1
    currentViz.save()

    # 17 is the beginning of the new cube viz's
    # @TODO: Change number for production
    if int(id) >= 2:
        source=SourceCode.objects.get(viz=currentViz)
        photo = False
    else:
        source = False;
        
    try:
        nextViz = currentViz.get_previous_by_created()
    except:
        nextViz=None

    if nextViz:
        return render(request, "viz/viz.html", { 'nextViz': nextViz, 'viz' : currentViz , 'photo':photo, 'binary':binary, 'comments': comments, 'source': source})    
    else:
        return render(request, "viz/viz.html", { 'viz' : currentViz , 'photo':photo, 'binary':binary, 'comments': comments, 'source': source})

''' Redeclaring above method; keeping it here for a while, until it's safe to remove
def viz(request, id):
    currentViz=Viz.objects.get(pk=id)
    try:
        binary=Binary.objects.get(viz=currentViz)
    except Binary.DoesNotExist:
        binary=None
    
    try:
        photo = Photo.objects.filter(viz=currentViz)[:1].get()  #get the main image associated with this viz, and use it as the photo
    except Photo.DoesNotExist:
        photo = False

    comments=Comment.objects.filter(viz=currentViz)
    currentViz.pageViews=currentViz.pageViews+1
    currentViz.save()

    # 17 is the beginning of the new cube viz's
    # @TODO: Change number for production
    if int(id) >= 2:
        source=SourceCode.objects.get(viz=currentViz)
        photo = False
    else:
        source = False;
        
    try:
        nextViz = currentViz.get_previous_by_created()
    except:
        nextViz=None

    if nextViz:
        return render(request, "viz/viz.html", { 'nextViz': nextViz, 'viz' : currentViz , 'photo':photo, 'binary':binary, 'comments': comments, 'source': source})    
    else:
        return render(request, "viz/viz.html", { 'viz' : currentViz , 'photo':photo, 'binary':binary, 'comments': comments, 'source': source})
'''
def vizText(request, id):
    currentViz=Viz.objects.get(pk=id)
    try:
        binary=Binary.objects.get(viz=currentViz)
    except Binary.DoesNotExist:
        binary=None
    
    try:
        photo = Photo.objects.filter(viz=currentViz)[:1].get()  #get the main image associated with this viz, and use it as the photo
    except Photo.DoesNotExist:
        photo = False

    comments=Comment.objects.filter(viz=currentViz)
    currentViz.pageViews=currentViz.pageViews+1
    currentViz.save()

    # 17 is the beginning of the new cube viz's
    # @TODO: Change number for production
    if int(id) >= 2:
        source=SourceCode.objects.get(viz=currentViz)
        photo = False
    else:
        source = False;
        
    try:
        nextViz = currentViz.get_previous_by_created()
    except:
        nextViz=None

    if nextViz:
        return render(request, "viz/vizText.html", { 'nextViz': nextViz, 'viz' : currentViz , 'photo':photo, 'binary':binary, 'comments': comments, 'source': source})    
    else:
        return render(request, "viz/vizText.html", { 'viz' : currentViz , 'photo':photo, 'binary':binary, 'comments': comments, 'source': source})



def vizTest(request, id):
    currentViz=Viz.objects.get(pk=id)
    try:
        binary=Binary.objects.get(viz=currentViz)
    except Binary.DoesNotExist:
        binary=None
    
    try:
        photo = Photo.objects.filter(viz=currentViz)[:1].get()  #get the main image associated with this viz, and use it as the photo
    except Photo.DoesNotExist:
        photo = False

    comments=Comment.objects.filter(viz=currentViz)
    currentViz.pageViews=currentViz.pageViews+1
    currentViz.save()

    # 17 is the beginning of the new cube viz's
    # @TODO: Change number for production
    if int(id) >= 2:
        source=SourceCode.objects.get(viz=currentViz)
        photo = False
    else:
        source = False;

    nextViz = currentViz.get_previous_by_created()
    if nextViz:
        return render(request, "viz/vizTest.html", { 'nextViz': nextViz, 'viz' : currentViz , 'photo':photo, 'binary':binary, 'comments': comments, 'source': source})    
    return render(request, "viz/vizTest.html", { 'viz' : currentViz , 'photo':photo, 'binary':binary, 'comments': comments, 'source': source})
    
def code(request, id):
    code=SourceCode.objects.get(pk=id)
    return HttpResponse(code.code, content_type="text/plain")

def create(request):
    return render(request, "viz/create.html")

def scroll(request, page=1, filter="newestFirst", cardsPerPage=8):
    page=int(page)
    if filter=="newestFirst":
        vizs=Viz.objects.all().exclude(published=False).order_by("-created")[page*cardsPerPage:(page+1)*cardsPerPage]
    else:
        vizs=Viz.objects.all().exclude(published=False).order_by("created")[page*cardsPerPage:(page+1)*cardsPerPage]

    if vizs.count() >= cardsPerPage:
        return render(request, "viz/gallery-page.html", { 'visualizations' : vizs , 'nextPage' : page+1, 'filter':filter})    
    else:
        return render(request, "viz/gallery-page.html", { 'visualizations' : vizs , 'nextPage' : False, 'filter':filter})    
   
    '''
    page=int(page)
    vizType=request.GET.get('vizType')
    if(vizType != None):
        vizs=Viz.objects.filter(vizType=vizType)[page*6:(page+1)*6]
    else:
        vizs=Viz.objects.all().order_by("-created")[page*6:(page+1)*6]
    return render(request, "viz/gallery-page.html", { 'visualizations' : vizs , 'nextPage' : page+1})    
    '''

def search(request, page=1, filter=None, cardsPerPage=8):
    page=int(page)
    if filter:
        try:
            vizUsers=CubeUser.objects.all().filter(nickname__icontains=filter).get()
        except CubeUser.DoesNotExist:
            vizUsers=None
        
        vizs=Viz.objects.none()
        if vizUsers:
            for user in vizUsers: 
                chain(vizs, Viz.objects.all().filter(creator=user.id))
        else:
            try:
                titleQuery=Viz.objects.all().filter(name__icontains=filter).exclude(published=False).order_by("-pageViews", "-created")
            except Viz.DoesNotExist:
                titleQuery=None
            try:
                descrQuery=Viz.objects.all().filter(description__icontains=filter).exclude(published=False).order_by("-pageViews", "-created")
            except Viz.DoesNotExist:
                descrQuery=None
            
            if titleQuery:
                chain(vizs, titleQuery)
            if descrQuery:
                chain(vizs, descrQuery)
            
    else: 
        # I've set this up to flag whether the URL 'filter' parameter is being passed from the js ajax
        # call and as it turns out it isn't... so the view always falls to this 'else' statement.
        vizs=None
        # This is what's actually going to happen once this bug is identified and squashed: 
        #Viz.objects.all().exclude(published=False).order_by("-pageViews", "-created")
    
    '''if vizs is None:
        totalObjects=0
    else:'''
    totalObjects=vizs.count()
    
    if totalObjects==0:
        return render(request, "viz/jsgallery.html", { 'visualizations' : None , 'nextPage' : False, 'totalObjects' : totalObjects, 'filter': filter })        
    elif totalObjects<cardsPerPage:
        visualizations=vizs[:totalObjects]
        return render(request, "viz/jsgallery.html", { 'visualizations' : visualizations , 'nextPage' : False, 'totalObjects' : totalObjects, 'filter': filter })
    else:
        visualizations=vizs[:cardsPerPage]
        return render(request, "viz/jsgallery.html", { 'visualizations' : visualizations , 'nextPage' : 1, 'totalObjects' : totalObjects, 'filter': filter })

def edit(request, id):
    try:
        viz=Viz.objects.get(pk=id)
        source=SourceCode.objects.get(viz=viz)
    except Viz.DoesNotExist:
        viz = None
    return render(request, "viz/create.html", { "viz": viz, "source": source} )


@csrf_exempt
def delete(request):
    nickname    = request.COOKIES['nickname']
    accessToken = request.COOKIES['accessToken']
    if authenticate(nickname, accessToken):
        try:
            vizId=request.POST['vizId']
            viz=Viz.objects.get(pk=vizId)
            viz.delete()
            return HttpResponse('{ "success": true }', content_type="application/json")
        except Viz.DoesNotExist:
            viz = None
            return HttpResponse('{ "success": false , "error" : "Viz %s does not exist" }' % vizId, content_type="application/json")
    else:
        return render(request, "viz/authentication-error.html", 
                      { "nickname": nickname,
                        "accessToken": accessToken,
                        "authenticated":authenticate(nickname, accessToken)})


@csrf_exempt
def save(request):
    nickname    = request.COOKIES['nickname']
    accessToken = request.COOKIES['accessToken']
    if authenticate(nickname, accessToken):
        vizID         = request.POST['vizId']
        name          = request.POST['name']
        description   = request.POST['description']
        code    = request.POST['sourceCode']
        vizType=request.POST['viz-type']
        videoURL=request.POST['videoURL']
        interactive     = request.POST['interactive']
        if interactive == 'false':
            interactive = False
        else:
            interactive = True

        published       = request.POST['published']
        if published == 'false':
            published = False
        else:
            published = True
        
        user=CubeUser.objects.get(accessToken=accessToken)
        viz=Viz.objects.get(pk=vizID)

        viz.name=name
        viz.description=description
        viz.interactive = interactive
        viz.published   = published
        viz.videoURL    = videoURL
        viz.vizType=vizType
        viz.save()

        newCode=SourceCode.objects.get(viz=viz)
        newCode.code=code
        newCode.save()

        return HttpResponse('{ "success": true , "id": "%s"}' % viz.pk, content_type="application/json")
    else:
        return render(request, "viz/authentication-error.html", 
                      { "nickname": nickname,
                        "accessToken": accessToken,
                        "authenticated":authenticate(nickname, accessToken)})

def authenticate(nickname, accessToken):
    authenticated=False
    try:
        user=CubeUser.objects.get(nickname=nickname)
        if user.accessToken==accessToken:
            authenticated=True
        else:
            user.accessToken=accessToken
            user.save()
            authenticated=True
    except CubeUser.DoesNotExist:
        authenticated=False
    return authenticated

@csrf_exempt
def upload(request):
    nickname    = request.COOKIES['nickname']
    accessToken = request.COOKIES['accessToken']
    # log.debug("%s is trying to edit or update a viz" % nickname)
    if authenticate(nickname, accessToken):
        # log.debug("authenticated!");
        try:
            id=request.POST['viz-id']
            log.debug("id=%s" % id)
            viz=Viz.objects.get(pk=id)
            log.debug("%s is updating %s" % (nickname, viz))
        except:
            viz=Viz()
            log.debug("%s is creating a new viz" % nickname)
            
        user            = CubeUser.objects.get(nickname=nickname)
        
        name            = request.POST['name']
        description     = request.POST['description']
        code            = request.POST['sourceCode']
        videoURL        = request.POST['videoURL']

        interactive     = request.POST['interactive']
        if interactive == 'false':
            interactive = False
        else:
            interactive = True

        published       = request.POST['published']
        if published == 'false':
            published = False
        else:
            published = True

        viz.name        = name
        viz.description = description
        viz.interactive = interactive
        viz.videoURL    = videoURL
        viz.published   = published

        viz.creator     = user
        viz.save()

        newCode = SourceCode()
        newCode.viz = viz
        newCode.code = code
        newCode.save()

        return HttpResponse('{ "success": true , "id": "%s"}' % viz.pk, content_type="application/json")
    else:
        return render(request, "viz/authentication-error.html", 
                      { "nickname": nickname,
                        "accessToken": accessToken,
                        "authenticated":authenticate(nickname, accessToken)})

@csrf_exempt
def flashWebsocketsListener(request, coreId, processor):
    accessToken=request.COOKIES.get('accessToken')
    log.debug("processor type is %s" % processor)
    if processor=="Photon":
        '''
        directory=os.path.join(settings.MEDIA_ROOT,'cloudware\\') #"/home/glass/cubetube-production/media/cloudware/"
        command=['node', 'directoryflash.js', '%s' % accessToken, '%s' % coreId, "photonListener"]
        log.debug(command)
        p = subprocess.Popen(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE, cwd=directory)
       
        jsonResult=""
        for line in p.stdout.readlines():
            line=line.replace('Device flashing started successfully: ', '')
            log.debug(line)
            jsonResult="%s%s" % (jsonResult, line)
        '''

        #media_root="/home/glass/cubetube-production/media/"
        #project_root="/home/glass/cubetube-production"    
        #log.debug('%s/viz/utils/flash.js' % project_root)
        directory=os.path.join(settings.PROJECT_ROOT, 'viz\\utils\\')   #"/home/glass/cubetube-production/media/cloudware/"
        binaryPath=os.path.join(settings.MEDIA_ROOT, "cloudware\\photonListener.bin")   #settings.WEBSOCKETS_LISTENER
        command=['node', '%sflash.js' % directory, accessToken, coreId, binaryPath]
        p = subprocess.Popen(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        #command=['node', '%s/viz/utils/flash.js' % settings.PROJECT_ROOT, accessToken, coreId, binaryPath]  #['node', 'flash.js', accessToken, coreId, binaryPath]
        #p = subprocess.Popen(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE, cwd=directory)
        log.debug("waiting for process to complete")
        retval = p.wait()
        log.debug("process completed")

        flash_output=[]
        flash_error=[]
        for line in p.stdout.readlines():
            print line,
            line.replace('"','\\"')
            line.replace("'","\\'")
            flash_output.append(line)
            
        for line in p.stderr.readlines():
            print line,
            line.replace('"','\\"')
            line.replace("'","\\'")
            flash_error.append(line)
            
        if flash_error:
            flash_error.append('PROJECT_ROOT: %s' % settings.PROJECT_ROOT)
            flash_error.append('directory: %s' % directory)
            flash_error.append('binaryPath: %s' % binaryPath)
            compilation_status="error"
            response={ "compilation_status": compilation_status , 
                       "flash_error" : flash_error}
        else:
            compilation_status="ok"
        
        if flash_output:
            response={ "compilation_status": compilation_status , 
                       "flash_output" : flash_output}
        return JsonResponse(response)
    else:
        binaryPath= settings.WEBSOCKETS_LISTENER
        flash_output=[]
        flash_error=[]

        #media_root="/home/glass/cubetube-production/media/"
        #project_root="/home/glass/cubetube-production"    
        #log.debug('%s/viz/utils/flash.js' % project_root)
        accessToken=request.COOKIES.get('accessToken')
        p = subprocess.Popen(['node', 'flash.js', accessToken, coreId, binaryPath], stdout=subprocess.PIPE, stderr=subprocess.PIPE)        #p = subprocess.Popen(['node', '%s/viz/utils/flash.js' % settings.PROJECT_ROOT, accessToken, coreId, binaryPath], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        #p = subprocess.Popen(['node', '%s/viz/utils/flash.js' % project_root,accessToken, coreId, binaryPath], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        retval = p.wait()

        for line in p.stdout.readlines():
            print line,
            line.replace('"','\\"')
            line.replace("'","\\'")
            flash_output.append(line)
            
        for line in p.stderr.readlines():
            print line,
            line.replace('"','\\"')
            line.replace("'","\\'")
            flash_error.append(line)
            
        compilation_status="ok"
#        retval = p.wait()
        response={ "compilation_status": compilation_status , 
                   "flash_output" : flash_output , 
                   "flash_error" : flash_error}
        return JsonResponse(response)
