# -*- coding: utf-8 -*-
import datetime
from south.db import db
from south.v2 import SchemaMigration
from django.db import models


class Migration(SchemaMigration):

    def forwards(self, orm):
        # Adding field 'Viz.pageViews'
        db.add_column(u'viz_viz', 'pageViews',
                      self.gf('django.db.models.fields.IntegerField')(default=0),
                      keep_default=False)


    def backwards(self, orm):
        # Deleting field 'Viz.pageViews'
        db.delete_column(u'viz_viz', 'pageViews')


    models = {
        u'cube.cubeuser': {
            'Meta': {'object_name': 'CubeUser'},
            'accessToken': ('django.db.models.fields.TextField', [], {}),
            'email': ('django.db.models.fields.TextField', [], {}),
            u'id': ('django.db.models.fields.AutoField', [], {'primary_key': 'True'}),
            'nickname': ('django.db.models.fields.TextField', [], {})
        },
        u'viz.binary': {
            'Meta': {'object_name': 'Binary'},
            'created': ('django.db.models.fields.DateTimeField', [], {'auto_now': 'True', 'blank': 'True'}),
            'file': ('django.db.models.fields.files.FileField', [], {'max_length': '100'}),
            u'id': ('django.db.models.fields.AutoField', [], {'primary_key': 'True'}),
            'viz': ('django.db.models.fields.related.ForeignKey', [], {'to': u"orm['viz.Viz']"})
        },
        u'viz.photo': {
            'Meta': {'object_name': 'Photo'},
            'file': ('django.db.models.fields.files.ImageField', [], {'max_length': '100', 'null': 'True', 'blank': 'True'}),
            u'id': ('django.db.models.fields.AutoField', [], {'primary_key': 'True'}),
            'viz': ('django.db.models.fields.related.ForeignKey', [], {'to': u"orm['viz.Viz']"})
        },
        u'viz.viz': {
            'Meta': {'object_name': 'Viz'},
            'created': ('django.db.models.fields.DateTimeField', [], {'auto_now_add': 'True', 'blank': 'True'}),
            'creator': ('django.db.models.fields.related.ForeignKey', [], {'to': u"orm['cube.CubeUser']"}),
            'description': ('django.db.models.fields.TextField', [], {}),
            u'id': ('django.db.models.fields.AutoField', [], {'primary_key': 'True'}),
            'name': ('django.db.models.fields.TextField', [], {}),
            'pageViews': ('django.db.models.fields.IntegerField', [], {'default': '0'}),
            'sourceURL': ('django.db.models.fields.TextField', [], {'default': "''"}),
            'tagline': ('django.db.models.fields.TextField', [], {}),
            'tags': ('django.db.models.fields.TextField', [], {'default': "''"}),
            'views': ('django.db.models.fields.IntegerField', [], {'default': '0'})
        }
    }

    complete_apps = ['viz']