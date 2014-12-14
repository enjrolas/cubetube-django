from django import template
from viz.models import Viz, Photo
register = template.Library()

@register.inclusion_tag('viz/thumbnail.html')
def thumbnail(viz):
    photo=Photo.objects.filter(viz=viz).filter(mainImage=True).get()  #get the main image associated with this viz, and use it as the photo
    return { 'viz' : viz, 'thumbnailPhoto' : photo }
