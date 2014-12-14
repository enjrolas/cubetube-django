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
            ('description', self.gf('django.db.models.fields.TextField')()),
            ('creator', self.gf('django.db.models.fields.related.ForeignKey')(to=orm['cube.CubeUser'])),
            ('created', self.gf('django.db.models.fields.DateTimeField')()),
            ('binaryURL', self.gf('django.db.models.fields.TextField')()),
            ('sourceURL', self.gf('django.db.models.fields.TextField')()),
        ))
        db.send_create_signal(u'viz', ['Viz'])

        # Adding model 'Photo'
        db.create_table(u'viz_photo', (
            (u'id', self.gf('django.db.models.fields.AutoField')(primary_key=True)),
            ('viz', self.gf('django.db.models.fields.related.ForeignKey')(to=orm['viz.Viz'])),
            ('photoURL', self.gf('django.db.models.fields.TextField')()),
            ('mainImage', self.gf('django.db.models.fields.BooleanField')(default=False)),
        ))
        db.send_create_signal(u'viz', ['Photo'])


    def backwards(self, orm):
        # Deleting model 'Viz'
        db.delete_table(u'viz_viz')

        # Deleting model 'Photo'
        db.delete_table(u'viz_photo')


    models = {
        u'cube.cubeuser': {
            'Meta': {'object_name': 'CubeUser'},
            'accessToken': ('django.db.models.fields.TextField', [], {}),
            u'id': ('django.db.models.fields.AutoField', [], {'primary_key': 'True'}),
            'user': ('django.db.models.fields.TextField', [], {})
        },
        u'viz.photo': {
            'Meta': {'object_name': 'Photo'},
            u'id': ('django.db.models.fields.AutoField', [], {'primary_key': 'True'}),
            'mainImage': ('django.db.models.fields.BooleanField', [], {'default': 'False'}),
            'photoURL': ('django.db.models.fields.TextField', [], {}),
            'viz': ('django.db.models.fields.related.ForeignKey', [], {'to': u"orm['viz.Viz']"})
        },
        u'viz.viz': {
            'Meta': {'object_name': 'Viz'},
            'binaryURL': ('django.db.models.fields.TextField', [], {}),
            'created': ('django.db.models.fields.DateTimeField', [], {}),
            'creator': ('django.db.models.fields.related.ForeignKey', [], {'to': u"orm['cube.CubeUser']"}),
            'description': ('django.db.models.fields.TextField', [], {}),
            u'id': ('django.db.models.fields.AutoField', [], {'primary_key': 'True'}),
            'name': ('django.db.models.fields.TextField', [], {}),
            'sourceURL': ('django.db.models.fields.TextField', [], {})
        }
    }

    complete_apps = ['viz']