import os
import sys
import site

#site.addsitedir('/home/tim/cubetube/env/lib/python2.7/site-packages')

# Add the app's directory to the PYTHONPATH
sys.path.append('/home/tim/cubetube')
sys.path.append('/home/tim/cubetube/cubetube')

os.environ['DJANGO_SETTINGS_MODULE'] = 'cubetube.settings'

# Activate your virtual env
#activate_env=os.path.expanduser("/home/tim/cubetube/env/bin/activate_this.py")
#execfile(activate_env, dict(__file__=activate_env))

from django.core.wsgi import get_wsgi_application
application = get_wsgi_application()