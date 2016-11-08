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
#from django.db.models.query import QuerySet
from django.db.models import Count
import calendar
#from datetime import date, timedelta
#from django.utils import timezone
from array import *
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
        vizs=Viz.objects.filter(vizType="L3D").order_by("-created").exclude(published=False)
    else:
        vizs=Viz.objects.filter(vizType="L3D").order_by("created").exclude(published=False)      

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
    vizLib=request.POST['viz-lib']
    
    if vizLib in [None, '']:
        queryViz=Viz.objects.get(pk=vizId)
        if queryViz==None:
            log.debug("Viz %i not found!" % vizId)
            return
        else:
            vizLib=queryViz.vizLib
    
    if vizId==None:
        vizId=-1
    else:
        vizId=int(vizId)
    log.debug(vizName);
    if vizName==None:
        vizName="undefined"

    if vizLib=="NEOPIXEL":
        code="%s\nchar* vizName=\"%s\";\nint vizId=%d;\n%s" % (settings.SPARK_LIBRARY, vizName, vizId, code)
    else:
        code="%s\nchar* vizName=\"%s\";\nint vizId=%d;\n%s" % (settings.FASTLED_LIBRARY, vizName, vizId, code)


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
    vizLib=request.POST['viz-lib']
    
    if vizId==None:
        vizId=-1
    else:
        vizId=int(vizId)
    log.debug(vizName);
    if vizName==None:
        vizName="undefined"
    
    if vizLib in [None, '']:
        queryViz=Viz.objects.get(pk=vizId)
        if queryViz==None:
            log.debug("Viz %i not found!" % vizId)
            return
        else:
            vizLib=queryViz.vizLib
    
    if code=="":
        queryViz=Viz.objects.get(pk=vizId)
        if queryViz==None:
            log.debug("Viz %i not found!" % vizId)
            return
        else:
            vizName=queryViz.name
            code=SourceCode.objects.get(viz=queryViz).code
        if code==None:
            log.debug("Code not found!" % vizId)
            return
    
    if vizLib=="NEOPIXEL":
        code="%s\n%s" % (settings.SPARK_LIBRARY, code)
    else:
        code="%s\n%s" % (settings.FASTLED_LIBRARY, code)

    lines=code.split('\n')
    i=0
    code=""
    for line in lines:
        code="%s\n%s" % (code, line)
        i+=1

    # Log user's activity in the db for stats
    user=CubeUser.objects.filter(accessToken=accessToken).get() # try to find the user who issued this viz's flash
    if user:    # if user is found, then update the table with the date
        user.lastActivity=datetime.datetime.now() # log the date this user has last done something
        user.lastPlaceVisited="FLASH: \"%s\"" % vizName.strip()
        user.save()
    
    # Log viz's activity in the db for stats
    queryViz=Viz.objects.get(pk=vizId)
    if queryViz==None:
        log.debug("Viz %i not found!" % vizId)
        return
    else:
        queryViz.views=queryViz.views+1              # we need to update the flash count
        queryViz.lastFlashed=datetime.datetime.now() # and the date it was last flashed
        queryViz.save()

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
    command=['node', 'cloudflash.js', '%s' % accessToken, '%s' % deviceID, '%s' % filename, '%s' % vizLib]
    log.debug(command)
    p = subprocess.Popen(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE, cwd=directory)
    jsonResult=""
    for line in p.stdout.readlines():
        jsonResult="%s%s" % (jsonResult, line)
        i+=1
    retval = p.wait() 
        
    return JsonResponse(jsonResult, safe=False)

@csrf_exempt
def flashSparkle(request):
    accessToken=request.POST['accessToken']
    deviceID=request.POST['deviceID']        
    directory=settings.CODE_DIRECTORY
    command=['node', 'flashSparkle.js', '%s' % accessToken, '%s' % deviceID]
    log.debug(command)
    p = subprocess.Popen(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE, cwd=directory)
    jsonResult=""
    for line in p.stdout.readlines():
        jsonResult="%s%s" % (jsonResult, line)
    retval = p.wait() 
        
    return JsonResponse(jsonResult, safe=False)

@csrf_exempt
def justCompile(request):
    code=request.POST['code']
    vizLib=request.POST['viz-lib']
    
    if vizLib in [None, '']:
        queryViz=Viz.objects.get(pk=vizId)
        if queryViz==None:
            log.debug("Viz %i not found!" % vizId)
            return
        else:
            vizLib=queryViz.vizLib
    
    if vizLib=="NEOPIXEL":
        code="%s\n%s" % (settings.SPARK_LIBRARY, code)
    else:
        code="%s\n%s" % (settings.FASTLED_LIBRARY, code)
    
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


#def jsgallery(request, filter="newestFirst", featuredViz=None, vizCreator=None):
def jsgallery(request, filter="newestFirst", filterTerm=None):
    featuredViz = None  #request.GET.get('featuredViz', None)
    accessToken=request.COOKIES.get('accessToken') 
    try:
        user=CubeUser.objects.filter(accessToken=accessToken).get()
    except CubeUser.DoesNotExist:
        user = None
    if user:
        userVizs=Viz.objects.filter(creator=user).order_by("-created")
        privateVizs=userVizs.filter(published=False).order_by("-created")
        publicVizs=userVizs.filter(vizType="L3D").filter(published=True).order_by("-created")
    else:
        privateVizs=None
        publicVizs=None

    cardsPerPage=8

    if(filter=='newestFirst'):
        if filterTerm:
            featuredViz = int(filterTerm)
        vizs=Viz.objects.filter(vizType="L3D").order_by("-averageRating", "-created").exclude(published=False)    
    elif(filter=='byCreator'):
        try:
            vizUser=CubeUser.objects.filter(nickname=filterTerm).first()
        except CubeUser.DoesNotExist:
            vizUser=None
        if(vizUser):        
            if(user):
                if(vizUser.nickname!=user.nickname):
                    vizs=Viz.objects.filter(vizType="L3D").filter(creator=vizUser.id).order_by("-created").exclude(published=False)
                else:
                    vizs=Viz.objects.filter(vizType="L3D").filter(creator=vizUser.id, published=True).order_by("-created")
            else:
                vizs=Viz.objects.filter(vizType="L3D").filter(creator=vizUser.id).order_by("-created").exclude(published=False)
        else:
            vizs=None      
    else:
        vizs=Viz.objects.filter(vizType="L3D").order_by("created").exclude(published=False)      
    
    if featuredViz is None:
        featured=None
    else:
        featured=Viz.objects.get(pk=featuredViz)

    if vizs is None:
        totalObjects=0
    else:
        if filter=="byCreator":
            totalObjects=vizs.count()
            cardsPerPage=totalObjects + 1
        else:
            totalObjects=vizs.count()
    
    if totalObjects==0:
        return render(request, "viz/jsgallery.html", { 'visualizations' : None , 'nextPage' : None, 'totalObjects' : totalObjects, 'filter': filter, 'featuredViz' : featured, 'privateVizs': privateVizs, 'publicVizs':publicVizs})        
    elif totalObjects < cardsPerPage:
        visualizations=vizs[:totalObjects]
        return render(request, "viz/jsgallery.html", { 'visualizations' : visualizations , 'nextPage' : None, 'totalObjects' : totalObjects, 'filter': filter, 'featuredViz' : featured, 'privateVizs': privateVizs, 'publicVizs':publicVizs})
    else:
        visualizations=vizs[:cardsPerPage]
        return render(request, "viz/jsgallery.html", { 'visualizations' : visualizations , 'nextPage' : 1, 'totalObjects' : totalObjects, 'filter': filter, 'featuredViz' : featured, 'privateVizs': privateVizs, 'publicVizs':publicVizs})

def index(request):
    log.debug("index")
#    vizs=Viz.objects.all().order_by("-created").exclude(published=False)
    vizs=Viz.objects.filter(vizType="L3D").order_by("-created").filter(featured=True).exclude(published=False)
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
        forked.name=viz.name.strip()
        forked.description=viz.description.strip()
        forked.vizType=viz.vizType;
        forked.vizLib=viz.vizLib;
        forked.creator=user
        forked.parent=viz
        forked.save()
        source=SourceCode.objects.filter(viz=viz)
        for code in source:
            newCode=SourceCode()
            newCode.viz=forked
            newCode.code=code.code
            newCode.save()
    
        # Log user's activity in the db for stats
        user.lastActivity=datetime.datetime.now() # log the date this user has last done something
        user.lastPlaceVisited="FORK: \"%s\"" % forked.name.strip()
        user.save()

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

def sparkle(request):
    return render(request, "viz/sparkle.html")
def sparkleGallery(request):
    return render(request, "viz/sparkleGallery.html", { 'visualizations' : None , 'nextPage' : None})        
def spark_pixels(request):
    return render(request, "viz/spark_pixels.html")
def cube_painter(request):
    return render(request, "viz/cube_painter.html")
def metrics(request): 
    return render(request, "viz/admin_stats.html")

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
        vizs=Viz.objects.filter(vizType="L3D").exclude(published=False).order_by("-averageRating", "-created")[page*cardsPerPage:(page+1)*cardsPerPage]
    elif filter=="all":
        vizs=Viz.objects.filter(vizType="L3D").exclude(published=False).order_by("-averageRating", "-created")
    else:
        vizs=Viz.objects.filter(vizType="L3D").exclude(published=False).order_by("-averageRating", "-created")[page*cardsPerPage:(page+1)*cardsPerPage]

    if vizs is None:
        totalObjects=0
    else:
        if filter=="all":
            totalObjects=vizs.count()
            cardsPerPage=totalObjects + 1
            page=0
        else:
            totalObjects=vizs.count()
            
    if totalObjects==0:
        return render(request, "viz/gallery-page.html", { 'visualizations' : None , 'nextPage' : False, 'filter':filter})
    elif totalObjects < cardsPerPage:
        visualizations=vizs[:totalObjects]
        return render(request, "viz/gallery-page.html", { 'visualizations' : visualizations , 'nextPage' : False, 'filter':filter, 'prevPage' : page-1})
    else:
        visualizations=vizs[:cardsPerPage]
        return render(request, "viz/gallery-page.html", { 'visualizations' : visualizations , 'nextPage' : page+1, 'filter':filter, 'prevPage' : page-1})    
   
    '''
    page=int(page)
    vizType=request.GET.get('vizType')
    if(vizType != None):
        vizs=Viz.objects.filter(vizType=vizType)[page*6:(page+1)*6]
    else:
        vizs=Viz.objects.all().order_by("-created")[page*6:(page+1)*6]
    return render(request, "viz/gallery-page.html", { 'visualizations' : vizs , 'nextPage' : page+1})    
    '''

def search(request, page=1, filter='*', cardsPerPage=8):
    page=int(page)
    cardsPerPage=int(cardsPerPage)
    vizs=Viz.objects.none()
    totalObjects=0
    
    if filter == '*':
        vizs=Viz.objects.filter(vizType="L3D").exclude(published=False).order_by("-averageRating", "-created")
    else:
        try:
            vizUsers=CubeUser.objects.all().filter(nickname__icontains=filter)
        except CubeUser.DoesNotExist:
            vizUsers=None
        
        if vizUsers:
            for user in vizUsers: 
                vizs=vizs | Viz.objects.filter(vizType="L3D").filter(creator=user.id).exclude(published=False)
        '''else:'''
        try:
            titleQuery=Viz.objects.filter(vizType="L3D").filter(name__icontains=filter).exclude(published=False).order_by("-averageRating", "-created")
        except Viz.DoesNotExist:
            titleQuery=None
        try:
            descrQuery=Viz.objects.filter(vizType="L3D").filter(description__icontains=filter).exclude(published=False).order_by("-averageRating", "-created")
        except Viz.DoesNotExist:
            descrQuery=None
        
        if titleQuery:
            vizs=vizs | titleQuery
        if descrQuery:
            vizs=vizs | descrQuery
    
    totalObjects=vizs.count()
    if filter != '*':
        cardsPerPage=totalObjects+1
    
    if totalObjects==0:
        return render(request, "viz/gallery-page.html", { 'visualizations' : None , 'nextPage' : None, 'totalObjects' : 0, 'filter':filter})
    elif totalObjects < cardsPerPage:
        visualizations=vizs[:totalObjects]
        return render(request, "viz/gallery-page.html", { 'visualizations' : visualizations , 'nextPage' : None, 'totalObjects' : totalObjects, 'filter':filter})
    else:
        visualizations=vizs[:cardsPerPage]
        return render(request, "viz/gallery-page.html", { 'visualizations' : visualizations , 'nextPage' : page+1, 'totalObjects' : totalObjects, 'filter':filter})

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
            
            # Log user's activity in the db for stats
            user=CubeUser.objects.filter(accessToken=accessToken).get() # try to find the user who issued this viz's flash
            if user:    # if user is found, then update the table with the date
                user.lastActivity=datetime.datetime.now() # log the date this user has last done something
                user.lastPlaceVisited="DELETE: \"%s\"" % viz.name.strip()
                user.save()
            
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
def rate(request):
    nickname    = request.COOKIES['nickname']
    accessToken = request.COOKIES['accessToken']
    vizId=request.POST['vizId']
    userVote=request.POST['rating']
    
    if authenticate(nickname, accessToken):
        try:
            viz=Viz.objects.get(pk=vizId)
            user=CubeUser.objects.get(nickname=nickname)
            userRatings=Rating.objects.filter(reviewer=user.id)
            
            # Log user's activity in the db for stats
            if user:    # if user is found, then update the table with the date
                user.lastActivity=datetime.datetime.now() # log the date this user has last done something
                user.lastPlaceVisited="RATE: \"%s\"" % viz.name.strip()
                user.save()
            
        except Viz.DoesNotExist:
            return HttpResponse('{ "success": false , "error" : "Viz %s does not exist" }' % vizId, content_type="application/json")
        except Rating.DoesNotExist:
            userRatings=None
        if userRatings:
            votedFor=userRatings.filter(viz=viz).count()
        else:
            votedFor=0
        if votedFor==0:
            allRatings=Rating.objects.filter(viz=viz)
            '''Weight each review computed for this Viz'''  
            fiveStars=(allRatings.filter(rating=5).count()+(1 if int(userVote)==5 else 0))
            fourStars=(allRatings.filter(rating=4).count()+(1 if int(userVote)==4 else 0))
            threeStars=(allRatings.filter(rating=3).count()+(1 if int(userVote)==3 else 0))
            twoStars=(allRatings.filter(rating=2).count()+(1 if int(userVote)==2 else 0))
            oneStar=(allRatings.filter(rating=1).count()+(1 if int(userVote)==1 else 0))
            
            viz.numberOfRatings+=1
            '''Sum of (weight * number of reviews at that weight) / total number of reviews'''
            viz.averageRating = int((5*fiveStars + 4*fourStars + 3*threeStars + 2*twoStars + oneStar)/viz.numberOfRatings)
            
            '''Create a new rating object and populate with new values'''
            rating=Rating()
            rating.viz=viz
            rating.reviewer=user
            rating.rating=int(userVote)
            rating.date=date.today()

            '''Save the new rating average for this Viz'''
            viz.save()
            '''Save the new user rating for this Viz'''
            rating.save()
            
            return HttpResponse('{ "success": true, "newAverage": "%s"}' % viz.averageRating, content_type="application/json")
        else:
            return HttpResponse('{ "success": false , "error" : "User %s already voted for Viz %s" }' % (nickname, vizId), content_type="application/json")
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
        code          = request.POST['sourceCode']
        vizType       = request.POST['viz-type']
        vizLib        = request.POST['viz-lib']
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

        viz.name=name.strip()
        viz.description=description.strip()
        viz.interactive = interactive
        viz.published   = published
        viz.videoURL    = videoURL
        viz.vizType     = vizType
        viz.vizLib      = vizLib
        viz.save()

        newCode=SourceCode.objects.get(viz=viz)
        newCode.code=code
        newCode.save()
        
        # Log user's activity in the db for stats
        if user:    # if user is found, then update the table with the date
            user.lastActivity=datetime.datetime.now() # log the date this user has last done something
            user.lastPlaceVisited="EDIT: \"%s\"" % viz.name.strip()
            user.save()

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
        vizLib          = request.POST['viz-lib']

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

        viz.name        = name.strip()
        viz.description = description.strip()
        viz.interactive = interactive
        viz.videoURL    = videoURL
        viz.published   = published
        viz.vizLib      = vizLib

        viz.creator     = user
        viz.save()

        newCode = SourceCode()
        newCode.viz = viz
        newCode.code = code
        newCode.save()
        
        # Log user's activity in the db for stats
        if user:    # if user is found, then update the table with the date
            user.lastActivity=datetime.datetime.now() # log the date this user has last done something
            user.lastPlaceVisited="CREATE: \"%s\"" % viz.name.strip()
            user.save()

        return HttpResponse('{ "success": true , "id": "%s"}' % viz.pk, content_type="application/json")
    else:
        return render(request, "viz/authentication-error.html", 
                      { "nickname": nickname,
                        "accessToken": accessToken,
                        "authenticated":authenticate(nickname, accessToken)})


@csrf_exempt
def log_event(request):
    event       = request.POST['event']
    accessToken = request.POST['accessToken']
    
    # Log user's activity in the db for stats
    user=CubeUser.objects.filter(accessToken=accessToken).get() # try to find the user who issued this event
    if user:    # if user is found, then update the table with the date
        #user.lastActivity=django.utils.timezone.now() # log the date this user has last done something
        user.lastActivity=datetime.datetime.now() # log the date this user has last done something
        user.lastPlaceVisited=event.strip()
        user.save()
    
        message = "Logged: \"%s\"" % user.lastPlaceVisited
        return JsonResponse({'success': 'true', 'message': message})
    else:
        error = "User with accessToken \"%s\" not found or invalid accessToken" % accessToken
        return JsonResponse({'success': 'false' , 'error': error})


@csrf_exempt
def viz_created(request):
    month       = request.GET['month']
    today       = datetime.datetime.today()
    next_year   = today.year if (int(month) < 12) else today.year + 1
    next_month  = int(month) + 1 if (int(month) < 12) else 1
    startDate = datetime.datetime.strptime("%s-%s-%s" % (today.year, int(month), 1),"%Y-%m-%d")
    endDate = datetime.datetime.strptime("%s-%s-%s" % (next_year, next_month, 1),"%Y-%m-%d")
    db_engine = settings.DATABASES['default']['ENGINE']
    
    log.debug("startDate: %s" % startDate.strftime('%Y-%m-%d'))
    log.debug("endDate: %s" % endDate.strftime('%Y-%m-%d'))
    if "sqlite" in db_engine:
        log.debug("SQL QUERY: %s" % Viz.objects.get_queryset().extra(select={'day':'STRFTIME(\'%%d\',created)','fmtCreated':'STRFTIME(\'%%m/%%d/%%Y\',created)'}, where=['vizType = \'%s\' AND published = true AND (created >= \'%s\' AND created < \'%s\')' % ("L3D", startDate.strftime('%Y-%m-%d'), endDate.strftime('%Y-%m-%d'))]).values('day').annotate(count=Count('pk')).values('fmtCreated','count').distinct().order_by("fmtCreated").query.__str__())
    else:
        log.debug("SQL QUERY: %s" % Viz.objects.get_queryset().extra(select={'day':'DATE_FORMAT(created,\'%%d\')','fmtCreated':'DATE_FORMAT(created,\'%%m/%%d/%%Y\')'}, where=['vizType = \'%s\' AND published = true AND (created >= \'%s\' AND created < \'%s\')' % ("L3D", startDate.strftime('%Y-%m-%d'), endDate.strftime('%Y-%m-%d'))]).values('day').annotate(count=Count('pk')).values('fmtCreated','count').distinct().order_by("fmtCreated").query.__str__())

    try:
        if "sqlite" in db_engine:
            grouped_query=Viz.objects.get_queryset().extra(select={'day':'STRFTIME(\'%%d\',created)','fmtCreated':'STRFTIME(\'%%m/%%d/%%Y\',created)'}, where=['vizType = \'%s\' AND published = true AND (created >= \'%s\' AND created < \'%s\')' % ("L3D", startDate.strftime('%Y-%m-%d'), endDate.strftime('%Y-%m-%d'))]).values('day').annotate(count=Count('pk')).values('fmtCreated','count').distinct().order_by("fmtCreated")
        else:
            grouped_query=Viz.objects.get_queryset().extra(select={'day':'DATE_FORMAT(created,\'%%d\')','fmtCreated':'DATE_FORMAT(created,\'%%m/%%d/%%Y\')'}, where=['vizType = \'%s\' AND published = true AND (created >= \'%s\' AND created < \'%s\')' % ("L3D", startDate.strftime('%Y-%m-%d'), endDate.strftime('%Y-%m-%d'))]).values('day').annotate(count=Count('pk')).values('fmtCreated','count').distinct().order_by("fmtCreated")

        series = []
        for item in grouped_query:
            #date = datetime.datetime.strptime(item['fmtCreated'], "%m/%d/%Y")
            #data = [calendar.timegm(date.timetuple()) * 1000, int(item['count'])]
            data = [item['fmtCreated'], int(item['count'])]
            series.append(data)

        response={ "label": "Viz created in %s, %d" % (calendar.month_name[int(month)], today.year) ,
                   "data" : series }
        return JsonResponse(response)
    except Exception as e:
        log.debug('QUERY ERROR > Message: %s, Type: %s, Args: [%s]' % (e.message, type(e), e.args))
        response={ "label": "error" ,
                   "data" : 'QUERY ERROR > Message: %s, Type: %s, Args: [%s]' % (e.message, type(e), e.args) }
        return JsonResponse(response)


@csrf_exempt
def viz_flashed(request):
    month       = request.GET['month']
    today       = datetime.datetime.today()
    next_year   = today.year if (int(month) < 12) else today.year + 1
    next_month  = int(month) + 1 if (int(month) < 12) else 1
    startDate = datetime.datetime.strptime("%s-%s-%s" % (today.year, int(month), 1),"%Y-%m-%d")
    endDate = datetime.datetime.strptime("%s-%s-%s" % (next_year, next_month, 1),"%Y-%m-%d")
    db_engine = settings.DATABASES['default']['ENGINE']
    
    log.debug("startDate: %s" % startDate.strftime('%Y-%m-%d'))
    log.debug("endDate: %s" % endDate.strftime('%Y-%m-%d'))
    if "sqlite" in db_engine:
        log.debug("SQL QUERY: %s" % Viz.objects.get_queryset().extra(select={'day':'STRFTIME(\'%%d\',lastFlashed)','fmtLastFlashed':'STRFTIME(\'%%m/%%d/%%Y\',lastFlashed)'}, where=['vizType = \'%s\' AND published = true AND (lastFlashed >= \'%s\' AND lastFlashed < \'%s\')' % ("L3D", startDate.strftime('%Y-%m-%d'), endDate.strftime('%Y-%m-%d'))]).values('day').annotate(count=Count('pk')).values('fmtLastFlashed','count').distinct().order_by("fmtLastFlashed").query.__str__())
    else:
        log.debug("SQL QUERY: %s" % Viz.objects.get_queryset().extra(select={'day':'DATE_FORMAT(lastFlashed,\'%%d\')','fmtLastFlashed':'DATE_FORMAT(lastFlashed,\'%%m/%%d/%%Y\')'}, where=['vizType = \'%s\' AND published = true AND (lastFlashed >= \'%s\' AND lastFlashed < \'%s\')' % ("L3D", startDate.strftime('%Y-%m-%d'), endDate.strftime('%Y-%m-%d'))]).values('day').annotate(count=Count('pk')).values('fmtLastFlashed','count').distinct().order_by("fmtLastFlashed").query.__str__())
        
    try:
        if "sqlite" in db_engine:
            grouped_query=Viz.objects.get_queryset().extra(select={'day':'STRFTIME(\'%%d\',lastFlashed)','fmtLastFlashed':'STRFTIME(\'%%m/%%d/%%Y\',lastFlashed)'}, where=['vizType = \'%s\' AND published = true AND (lastFlashed >= \'%s\' AND lastFlashed < \'%s\')' % ("L3D", startDate.strftime('%Y-%m-%d'), endDate.strftime('%Y-%m-%d'))]).values('day').annotate(count=Count('pk')).values('fmtLastFlashed','count').distinct().order_by("fmtLastFlashed")
        else:
            grouped_query=Viz.objects.get_queryset().extra(select={'day':'DATE_FORMAT(lastFlashed,\'%%d\')','fmtLastFlashed':'DATE_FORMAT(lastFlashed,\'%%m/%%d/%%Y\')'}, where=['vizType = \'%s\' AND published = true AND (lastFlashed >= \'%s\' AND lastFlashed < \'%s\')' % ("L3D", startDate.strftime('%Y-%m-%d'), endDate.strftime('%Y-%m-%d'))]).values('day').annotate(count=Count('pk')).values('fmtLastFlashed','count').distinct().order_by("fmtLastFlashed")
        
        series = []
        for item in grouped_query:
            #date = datetime.datetime.strptime(item['fmtLastFlashed'], "%m/%d/%Y")
            #data = [calendar.timegm(date.timetuple()) * 1000, int(item['count'])]
            data = [item['fmtLastFlashed'], int(item['count'])]
            series.append(data)

        response={ "label": "Viz flashed in %s, %d" % (calendar.month_name[int(month)], today.year) ,
                   "data" : series }
        return JsonResponse(response)
    except Exception as e:
        log.debug('QUERY ERROR > Message: %s, Type: %s, Args: [%s]' % (e.message, type(e), e.args))
        response={ "label": "error" ,
                   "data" : 'QUERY ERROR > Message: %s, Type: %s, Args: [%s]' % (e.message, type(e), e.args) }
        return JsonResponse(response)


@csrf_exempt
def unique_daily_users(request):
    month       = request.GET['month']
    today       = datetime.datetime.today()
    next_year   = today.year if (int(month) < 12) else today.year + 1
    next_month  = int(month) + 1 if (int(month) < 12) else 1
    startDate = datetime.datetime.strptime("%s-%s-%s" % (today.year, int(month), 1),"%Y-%m-%d")
    endDate = datetime.datetime.strptime("%s-%s-%s" % (next_year, next_month, 1),"%Y-%m-%d")
    db_engine = settings.DATABASES['default']['ENGINE']
    
    log.debug("startDate: %s" % startDate.strftime('%Y-%m-%d'))
    log.debug("endDate: %s" % endDate.strftime('%Y-%m-%d'))
    if "sqlite" in db_engine:
        log.debug("SQL QUERY: %s" % CubeUser.objects.get_queryset().extra(select={'day':'STRFTIME(\'%%d\',lastActivity)','fmtLastActivity':'STRFTIME(\'%%m/%%d/%%Y\',lastActivity)'}, where=['(lastActivity >= \'%s\' AND lastActivity < \'%s\')' % (startDate.strftime('%Y-%m-%d'), endDate.strftime('%Y-%m-%d'))]).values('day').annotate(count=Count('pk')).values('fmtLastActivity','count').distinct().order_by("fmtLastActivity").query.__str__())
    else:
        log.debug("SQL QUERY: %s" % CubeUser.objects.get_queryset().extra(select={'day':'DATE_FORMAT(lastActivity,\'%%d\')','fmtLastActivity':'DATE_FORMAT(lastActivity,\'%%m/%%d/%%Y\')'}, where=['(lastActivity >= \'%s\' AND lastActivity < \'%s\')' % (startDate.strftime('%Y-%m-%d'), endDate.strftime('%Y-%m-%d'))]).values('day').annotate(count=Count('pk')).values('fmtLastActivity','count').distinct().order_by("fmtLastActivity").query.__str__())
    
    try:
        if "sqlite" in db_engine:
            grouped_query=CubeUser.objects.get_queryset().extra(select={'day':'STRFTIME(\'%%d\',lastActivity)','fmtLastActivity':'STRFTIME(\'%%m/%%d/%%Y\',lastActivity)'}, where=['(lastActivity >= \'%s\' AND lastActivity < \'%s\')' % (startDate.strftime('%Y-%m-%d'), endDate.strftime('%Y-%m-%d'))]).values('day').annotate(count=Count('pk')).values('fmtLastActivity','count').distinct().order_by("fmtLastActivity")
        else:
            grouped_query=CubeUser.objects.get_queryset().extra(select={'day':'DATE_FORMAT(lastActivity,\'%%d\')','fmtLastActivity':'DATE_FORMAT(lastActivity,\'%%m/%%d/%%Y\')'}, where=['(lastActivity >= \'%s\' AND lastActivity < \'%s\')' % (startDate.strftime('%Y-%m-%d'), endDate.strftime('%Y-%m-%d'))]).values('day').annotate(count=Count('pk')).values('fmtLastActivity','count').distinct().order_by("fmtLastActivity")
    
        series = []
        for item in grouped_query:
            #date = datetime.datetime.strptime(item['fmtlastActivity'], "%m/%d/%Y")
            #data = [calendar.timegm(date.timetuple()) * 1000, int(item['count'])]
            data = [item['fmtLastActivity'], int(item['count'])]
            series.append(data)

        response={ "label": "Users in %s, %d" % (calendar.month_name[int(month)], today.year) ,
                   "data" : series }
        return JsonResponse(response)
    except Exception as e:
        log.debug('QUERY ERROR > Message: %s, Type: %s, Args: [%s]' % (e.message, type(e), e.args))
        response={ "label": "error" ,
                   "data" : 'QUERY ERROR > Message: %s, Type: %s, Args: [%s]' % (e.message, type(e), e.args) }
        return JsonResponse(response)


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
