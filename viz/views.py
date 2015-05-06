from django.shortcuts import get_object_or_404, render, redirect
from viz.models import *
from cube.models import CubeUser
from comments.models import Comment
from django.http import HttpResponse
from django.utils.html import escape
from django.conf import settings
from django.views.decorators.csrf import csrf_exempt
import logging
log = logging.getLogger(__name__)

def parameter(request, param):
    return HttpResponse("param is: {}".format(param))

def gallery(request, filter="newestFirst"):
    if(filter=='newestFirst'):
        vizs=Viz.objects.all().order_by("-created")    
    else:
        vizs=Viz.objects.all().order_by("created")      

    totalObjects=vizs.count()
    if totalObjects<12:
        visualizations=vizs[:totalObjects]
    else:
        visualizations=vizs[:12]
    return render(request, "viz/gallery.html", { 'visualizations' : visualizations , 'nextPage' : 1, 'totalObjects' : totalObjects, 'filter': filter})

def jsgallery(request, filter="newestFirst"):
    if(filter=='newestFirst'):
        vizs=Viz.objects.all().order_by("-created")    
    else:
        vizs=Viz.objects.all().order_by("created")      

    totalObjects=vizs.count()
    if totalObjects<12:
        visualizations=vizs[:totalObjects]
    else:
        visualizations=vizs[:12]
    return render(request, "viz/jsgallery.html", { 'visualizations' : visualizations , 'nextPage' : 1, 'totalObjects' : totalObjects, 'filter': filter})

def index(request):
    vizs=Viz.objects.all().order_by("-created")    
    totalObjects=vizs.count()
    if totalObjects<12:
        visualizations=vizs[:totalObjects]
    else:
        visualizations=vizs[:12]
    return render(request, "viz/index.html", { 'visualizations' : visualizations, 'totalObjects' : totalObjects})

@csrf_exempt
def fork(request):
    accessToken=request.POST['accessToken']
    vizId=request.POST['vizId']
    try:
        user=CubeUser.objects.filter(accessToken=accessToken).get()
    except CubeUser.DoesNotExist:
        user = None
    if user:
        viz=Viz.objects.get(pk=vizId)
        forked=Viz()
        forked.name=viz.name
        forked.tagline=viz.tagline
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

        return HttpResponse('{"id":%d}'%forked.pk, content_type="application/json")

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

    nextViz = currentViz.get_previous_by_created()
    if nextViz:
        return render(request, "viz/viz.html", { 'nextViz': nextViz, 'viz' : currentViz , 'photo':photo, 'binary':binary, 'comments': comments, 'source': source})    
    return render(request, "viz/viz.html", { 'viz' : currentViz , 'photo':photo, 'binary':binary, 'comments': comments, 'source': source})
    
    # if currentViz.vizType=="streaming":
    #     binary=Binary.objects.get(pk=settings.LISTENER_PK)
    # if currentViz.vizType=='javascript':
    #     source=SourceCode.objects.get(viz=currentViz)
    #     return render(request, "viz/javascript.html", { 'viz' : currentViz , 'photos':photos, 'binary':binary, 'comments': comments, 'source':source})
    # elif currentViz.vizType=='L3D':
    #     source=SourceCode.objects.get(viz=currentViz)
    #     return render(request, "viz/sparkPreview.html", { 'viz' : currentViz , 'photos':photos, 'binary':binary, 'comments': comments, 'source':source})
    # else:
    #     return render(request, "viz/detail.html", { 'viz' : currentViz , 'photos':photos, 'binary':binary, 'comments': comments})

def code(request, id):
    code=SourceCode.objects.get(pk=id)
    return HttpResponse(code.code, content_type="text/plain")

def create(request):
    return render(request, "viz/create.html")

def scroll(request, page, filter="newestFirst"):
    page=int(page)
    if filter=="newestFirst":
        vizs=Viz.objects.all().order_by("-created")[page*6:(page+1)*6]
    else:
        vizs=Viz.objects.all().order_by("created")[page*6:(page+1)*6]
    return render(request, "viz/gallery-page.html", { 'visualizations' : vizs , 'nextPage' : page+1, 'filter':filter})    
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
    except Viz.DoesNotExist:
        viz = None
    return render(request, "viz/create.html", { "viz": viz} )

def save(request):
    accessToken=request.POST['accessToken']
    vizID=request.POST['vizID']
    name=request.POST['name']
    description=request.POST['description']
    sourceCode=request.POST['sourceCode']
    interactive = request.POST['interactive']
    videoUrl = request.POST['interactive']

    user=CubeUser.objects.get(accessToken=accessToken)
    viz=Viz.objects.get(pk=vizID)
    viz.name        = name
    viz.description = description
    viz.interactive = interactive
    viz.videoUrl    = videoUrl
    viz.save()

    code=SourceCode.get(viz=viz)
    code.code=sourceCode
    code.save()
    return HttpResponse("ok")

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

def upload(request):
    nickname=request.COOKIES['nickname']
    accessToken=request.COOKIES['accessToken']
    log.debug("%s is trying to edit or update a viz" % nickname)
    if authenticate(nickname, accessToken):
        log.debug("authenticated!");
        try:
            id=request.POST['viz-id']
            log.debug("id=%s" % id)
            viz=Viz.objects.get(pk=id)
            log.debug("%s is updating %s" % (nickname, viz))
        except:
            viz=Viz()
            log.debug("%s is creating a new viz" % nickname)
            
        user=CubeUser.objects.get(nickname=nickname)
        viz.name=request.POST['viz-name']
        viz.tagline=request.POST['viz-tagline']
        viz.description=request.POST['viz-description']
        log.debug("description: %s" % viz.description)
        viz.vizType=request.POST['viz-type']
        viz.sourceURL=request.POST['viz-source-link']
        viz.tags=request.POST['viz-tags']
        viz.creator=user
        log.debug("viz type is :%s" % viz.vizType)
        viz.save()
        for fileName in request.FILES.getlist('photo'):
            photo=Photo()
            photo.viz=viz
            photo.file=fileName
            photo.save()
            
        for fileName in request.FILES.getlist('viz-binary'):
            try:
                binary=Binary.objects.get(viz=viz)
                log.debug("binary file already exists - %s" % binary)
            except Binary.DoesNotExist:              
                log.debug("no binary file yet -- just created one")
                binary=Binary()
            binary.file=fileName
            binary.viz=viz
            binary.save()
            
        photos=Photo.objects.filter(viz=viz)
        #    return HttpResponse(escape(repr(request)))
        #    return render(request, "viz/debug.html", {'files': request.FILES})
        return redirect('viz', id=viz.pk)
    else:
        return render(request, "viz/authentication-error.html", 
                      { "nickname": nickname,
                        "accessToken": accessToken,
                        "authenticated":authenticate(nickname, accessToken)})

def photoUpload(request):
    return render(request, "viz/create.html")
    
