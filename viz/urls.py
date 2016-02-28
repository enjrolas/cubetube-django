from django.conf.urls import url
from django.views.generic import RedirectView
from viz import views

urlpatterns = [
    url(r'^$', views.index, name='index'),
    #url(r'^gallery/(?P<filter>\w+)/(?P<vizCreator>[\w.,!?:_-]+)/$', views.jsgallery, name='gallery'),
    #url(r'^gallery/(?P<filter>\w+)/(?P<featuredViz>\d+)/$', views.jsgallery, name='gallery'),
    url(r'^gallery/(?P<filter>\w+)/(?P<filterTerm>[\w.,!?:_-]+)/$', views.jsgallery, name='gallery'),
    url(r'^gallery/(?P<filter>\w+)/$', views.jsgallery, name='gallery'),
    url(r'^gallery/', views.jsgallery, name='gallery'),
    url(r'^jsgallery/', views.jsgallery, name='jsgallery'),    
    url(r'^sparkle/', views.sparkleGallery, name='sparkle'),    

    url(r'^viz/(?P<id>\d+)/$', views.viz, name='viz'),
    url(r'^sparkleViz/$', views.sparkle, name='sparkleViz'),
    url(r'^spark_pixels/$', views.spark_pixels, name='spark_pixels'),
    url(r'^cube_painter/$', views.cube_painter, name='cube_painter'),
    url(r'^vizText/(?P<id>\d+)/$', views.vizText, name='vizText'),
    url(r'^scroll/(?P<page>\d+)/(?P<filter>\w+)/(?P<cardsPerPage>\d+)/$', views.scroll, name='scroll'),
    url(r'^scroll/(?P<page>\d+)/(?P<filter>\w+)/$', views.scroll, name='scroll'),
    url(r'^scroll/(?P<page>\d+)/$', views.scroll, name='scroll'), 
    url(r'^scroll/(?P<filter>\w+)/(?P<cardsPerPage>\d+)/$', views.scroll, name='scroll'),
    url(r'^fork/(?P<vizId>\d+)/$', views.fork, name='fork'),
    url(r'^compile/', views.compile, name='compile'),
    url(r'^cloudFlash/', views.cloudFlash, name='cloudFlash'),
    url(r'^flashSparkle/', views.flashSparkle, name='flashSparkle'),
    url(r'^filter/', views.filter, name='filter'),
    url(r'^appInfo/', views.appInfo, name='appInfo'),
    url(r'^justCompile/', views.justCompile, name='justCompile'),
    url(r'^listener/(?P<coreId>\w+)/(?P<processor>\w+)/$', views.flashWebsocketsListener, name='flashWebsocketsListener'),
    url(r'^save/$', views.save, name='save'),
    url(r'^code/(?P<id>\d+)/$', views.code, name='code'),
    url(r'^create/$', views.create, name='create'),
    url(r'^upload/$', views.upload, name='upload'),
    url(r'^edit/(?P<id>\d+)/$', views.edit, name='edit'),
<<<<<<< HEAD
    url(r'^delete/$', views.delete, name='delete'),
=======
    url(r'^delete/', views.delete, name='delete'),
    url(r'^rate/', views.rate, name='rate'),
>>>>>>> development
    url(r'^search/$', views.search, name='search'),
    url(r'^search/(?P<page>\d+)/$', views.search, name='search'),
    url(r'^search/(?P<filter>\w+)/$', views.search, name='search'),
    url(r'^search/(?P<page>\d+)/(?P<filter>\w+)/$', views.search, name='search'),
    url(r'^search/(?P<filter>\w+)/(?P<cardsPerPage>\d+)/$', views.search, name='search'),
    url(r'^search/(?P<page>\d+)/(?P<filter>\w+)/(?P<cardsPerPage>\d+)/$', views.search, name='search'),
]
