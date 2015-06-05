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
    media_root="/home/glass/cubetube-testing/media/"
    project_root="/home/glass/cubetube-testing/"
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
    media_root="/home/glass/cubetube-testing/media/"
    project_root="/home/glass/cubetube-testing/"
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

def jsgallery(request, filter="newestFirst", featuredViz=None):
    accessToken=request.COOKIES.get('accessToken') 
    try:
        user=CubeUser.objects.filter(accessToken=accessToken).get()
    except CubeUser.DoesNotExist:
        user = None
    if user:
        userVizs=Viz.objects.filter(creator=user)
        privateVizs=userVizs.filter(published=False)
        publicVizs=userVizs.filter(published=True)
    else:
        privateVizs=None
        publicVizs=None

    if(filter=='newestFirst'):
        vizs=Viz.objects.all().order_by("-created").exclude(published=False)    
    else:
        vizs=Viz.objects.all().order_by("created").exclude(published=False)      
    
    if featuredViz is None:
        featured=None
    else:
        featured=Viz.objects.get(pk=featuredViz)

    totalObjects=vizs.count()
    if totalObjects<8:
        visualizations=vizs[:totalObjects]
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
        raise Http404 

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

def scroll(request, page, filter="newestFirst", cardsPerPage=8):
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

def edit(request, id):
    try:
        viz=Viz.objects.get(pk=id)
        source=SourceCode.objects.get(viz=viz)
    except Viz.DoesNotExist:
        viz = None
    return render(request, "viz/create.html", { "viz": viz, "source": source} )

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
        
        videoUrl      = request.POST['videoURL']

        user=CubeUser.objects.get(accessToken=accessToken)
        viz=Viz.objects.get(pk=vizID)

        viz.name=name
        viz.description=description
        viz.interactive = interactive
        viz.published   = published
        viz.videoUrl    = videoUrl
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
def flashWebsocketsListener(request, coreId):
    binaryPath= settings.WEBSOCKETS_LISTENER
    flash_output=[]
    flash_error=[]

    media_root="/home/glass/cubetube-testing/media/"
    project_root="/home/glass/cubetube-testing"    
    log.debug('%s/viz/utils/flash.js' % project_root)
    accessToken=request.COOKIES.get('accessToken')
    p = subprocess.Popen(['node', '%s/viz/utils/flash.js' % project_root,accessToken, coreId, binaryPath], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
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
    response={ "compilation_status": compilation_status , 
               "flash_output" : flash_output , 
               "flash_error" : flash_error}
    return JsonResponse(response)
