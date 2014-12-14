import os
import sys
import site

# virtual environment paths site.addsitedir('/home/<sitehome>/virtualenv/.../lib/python2.7/site-packages/')
site.addsitedir('/home/glass/cubetube/cubetube/lib/python2.7/site-packages/')
path = '/home/glass/cubetube/'

if path not in sys.path:
    sys.path.append(path)

os.environ['DJANGO_SETTINGS_MODULE'] = 'cubetube.settings'

import django.core.handlers.wsgi
application = django.core.handlers.wsgi.WSGIHandler()