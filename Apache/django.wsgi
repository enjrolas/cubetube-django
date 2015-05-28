import os
import sys
import site

#site.addsitedir('/home/tim/cubetube/env/lib/python2.7/site-packages')

# Add the app's directory to the PYTHONPATH
sys.path.append('/home/glass/cubetube-testing')
sys.path.append('/home/glass/cubetube-testing/cubetube')

os.environ['DJANGO_SETTINGS_MODULE'] = 'cubetube.settings'

from django.core.wsgi import get_wsgi_application
application = get_wsgi_application()