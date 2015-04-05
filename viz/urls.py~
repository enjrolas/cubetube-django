from django.conf.urls import url
from viz import views

urlpatterns = [
    url(r'^$', views.gallery, name='gallery'),
    url(r'^viz/(?P<id>\d+)/$', views.viz, name='viz'),
    url(r'^scroll/(?P<page>\d+)/$', views.scroll, name='scroll'),
    url(r'^fork/', views.fork, name='fork'),
    url(r'^save/', views.save, name='save'),
    url(r'^create/', views.create, name='create'),
    url(r'^edit/(?P<id>\d+)/$', views.edit, name='edit'),
    url(r'^upload/', views.upload, name='upload'),
    url(r'^photoUpload/', views.photoUpload, name='photo-upload'),
]
