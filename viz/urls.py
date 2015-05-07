from django.conf.urls import url
from django.views.generic import RedirectView
from viz import views

urlpatterns = [
    url(r'^$', views.index, name='index'),

    url(r'^parameter/(?P<param>\w+)/$', views.parameter, name='parameter'),
    url(r'^parameter/$', views.parameter, name='parameter'),
    url(r'^gallery/(?P<filter>\w+)/$', views.jsgallery, name='gallery'),
    url(r'^gallery/', views.jsgallery, name='gallery'),
    url(r'^jsgallery/', views.jsgallery, name='jsgallery'),    
    url(r'^viz/(?P<id>\d+)/$', views.viz, name='viz'),
    url(r'^scroll/(?P<page>\d+)/(?P<filter>\w+)/$', views.scroll, name='scroll'),
    url(r'^scroll/(?P<page>\d+)/$', views.scroll, name='scroll'), 
    url(r'^fork/', views.fork, name='fork'),
    url(r'^compile/', views.compile, name='compile'),
    url(r'^save/', views.save, name='save'),
    url(r'^code/(?P<id>\d+)/$', views.code, name='code'),
    url(r'^create/', views.create, name='create'),
    url(r'^upload/', views.upload, name='upload'),
    url(r'^edit/(?P<id>\d+)/$', views.edit, name='edit'),
]
