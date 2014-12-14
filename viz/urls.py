from django.conf.urls import url
from viz import views

urlpatterns = [
    url(r'^gallery/', views.gallery, name='gallery'),
    url(r'^viz/(?P<id>\d+)/$', views.viz, name='viz'),
]
