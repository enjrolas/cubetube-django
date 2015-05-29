import os
import sys
import site

# Add the app's directory to the PYTHONPATH
#sys.path.append('/home/glass/cubetube-testing')
#sys.path.append('/home/glass/cubetube-testing/cubetube')

sys.path.append('/home/glass/cubetube-production')
sys.path.append('/home/glass/cubetube-production/cubetube')

os.environ['DJANGO_SETTINGS_MODULE'] = 'cubetube.settings'

from django.core.wsgi import get_wsgi_application
application = get_wsgi_application()