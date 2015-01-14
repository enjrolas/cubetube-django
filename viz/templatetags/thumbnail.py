from django.core.exceptions import ObjectDoesNotExist
from django import template
from viz.models import Viz, Photo
register = template.Library()

@register.inclusion_tag('viz/thumbnail.html')
def thumbnail(viz, nextPage=None, vizType=None):
    try:
        photo=Photo.objects.filter(viz=viz)[:1]  #get the main image associated with this viz, and use it as the photo
        photo=photo[0]
    except:
        photo=None
    if nextPage is not None:
            return { 'viz' : viz, 'thumbnailPhoto' : photo, 'nextPage' : nextPage, 'vizType' : vizType }
    else:
        return { 'viz' : viz, 'thumbnailPhoto' : photo }

