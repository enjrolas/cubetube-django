from django.shortcuts import get_object_or_404, render
from viz.models import Viz, Photo

def gallery(request):
    visualizations=Viz.objects.all()
    return render(request, "viz/gallery.html", { 'visualizations' : visualizations })

def viz(request, id):
    currentViz=Viz.objects.get(pk=id)
    photos=Photo.objects.filter(viz=currentViz)  #get the main image associated with this viz, and use it as the photo
    return render(request, "viz/detail.html", { 'viz' : currentViz , 'photos':photos})
