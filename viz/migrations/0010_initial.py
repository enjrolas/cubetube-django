# -*- coding: utf-8 -*-
import datetime
from south.db import db
from south.v2 import SchemaMigration
from django.db import models


class Migration(SchemaMigration):

    def forwards(self, orm):
        # Adding model 'Viz'
        db.create_table(u'viz_viz', (
            (u'id', self.gf('django.db.models.fields.AutoField')(primary_key=True)),
            ('name', self.gf('django.db.models.fields.TextField')()),
            ('tagline', self.gf('django.db.models.fields.TextField')()),
            ('description', self.gf('django.db.models.fields.TextField')()),
            ('creator', self.gf('django.db.models.fields.related.ForeignKey')(to=orm['cube.CubeUser'])),
            ('created', self.gf('django.db.models.fields.DateTimeField')(auto_now_add=True, blank=True)),
            ('sourceURL', self.gf('django.db.models.fields.TextField')()),
        ))
        db.send_create_signal(u'viz', ['Viz'])

        # Adding model 'Photo'
        db.create_table(u'viz_photo', (
            (u'id', self.gf('django.db.models.fields.AutoField')(primary_key=True)),
            ('viz', self.gf('django.db.models.fields.related.ForeignKey')(to=orm['viz.Viz'])),
            ('file', self.gf('django.db.models.fields.files.ImageField')(max_length=100, null=True, blank=True)),
        ))
        db.send_create_signal(u'viz', ['Photo'])

        # Adding model 'Binary'
        db.create_table(u'viz_binary', (
            (u'id', self.gf('django.db.models.fields.AutoField')(primary_key=True)),
            ('file', self.gf('django.db.models.fields.files.FileField')(max_length=100)),
            ('viz', self.gf('django.db.models.fields.related.ForeignKey')(to=orm['viz.Viz'])),
            ('created', self.gf('django.db.models.fields.DateTimeField')(auto_now=True, blank=True)),
        ))
        db.send_create_signal(u'viz', ['Binary'])


    def backwards(self, orm):
        # Deleting model 'Viz'
        db.delete_table(u'viz_viz')

        # Deleting model 'Photo'
        db.delete_table(u'viz_photo')

        # Deleting model 'Binary'
        db.delete_table(u'viz_binary')


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
            'sourceURL': ('django.db.models.fields.TextField', [], {}),
            'tagline': ('django.db.models.fields.TextField', [], {})
        }
    }

    complete_apps = ['viz']