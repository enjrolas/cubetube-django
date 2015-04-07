from django.core.exceptions import ObjectDoesNotExist
from django import template
from viz.models import Viz, Photo
from django.conf import settings
import logging

register = template.Library()
log = logging.getLogger(__name__)

@register.inclusion_tag('viz/card.html')
def card(viz, nextPage=None):
    try:
        photo = Photo.objects.filter(viz=viz)[:1]
        photo = photo[0]
        thumbnailPhotoURL = photo.file.url
    except Exception,e:
        photo = None
        thumbnailPhotoURL = None
        log.debug(e)
    return { 'viz': viz, 'thumbnailPhoto': photo , 'thumbnailPhotoURL': thumbnailPhotoURL }