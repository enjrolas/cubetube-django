from django.core.urlresolvers import reverse
from django.conf import settings # import the settings file

def listeners(request):
    flashWebsocketsListener=reverse('flashWebsocketsListener')
    # return the value you want as a dictionnary. you may add multiple values in there.
    return {'WEBSOCKETS_LISTENER': settings.WEBSOCKETS_LISTENER, 'FLASH_WEBSOCKETS_LISTENER': flashWebsocketsListener}
