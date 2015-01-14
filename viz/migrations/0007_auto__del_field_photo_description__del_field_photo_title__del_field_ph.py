# -*- coding: utf-8 -*-
import datetime
from south.db import db
from south.v2 import SchemaMigration
from django.db import models


class Migration(SchemaMigration):

    def forwards(self, orm):
        # Deleting field 'Photo.description'
        db.delete_column(u'viz_photo', 'description')

        # Deleting field 'Photo.title'
        db.delete_column(u'viz_photo', 'title')

        # Deleting field 'Photo.photoURL'
        db.delete_column(u'viz_photo', 'photoURL')

        # Deleting field 'Photo.mainImage'
        db.delete_column(u'viz_photo', 'mainImage')

        # Adding field 'Photo.file'
        db.add_column(u'viz_photo', 'file',
                      self.gf('django.db.models.fields.files.FileField')(default=' ', max_length=100),
                      keep_default=False)

        # Deleting field 'Viz.binaryURL'
        db.delete_column(u'viz_viz', 'binaryURL')

        # Adding field 'Binary.file'
        db.add_column(u'viz_binary', 'file',
                      self.gf('django.db.models.fields.files.FileField')(default=' ', max_length=100),
                      keep_default=False)


    def backwards(self, orm):
        # Adding field 'Photo.description'
        db.add_column(u'viz_photo', 'description',
                      self.gf('django.db.models.fields.TextField')(null=True),
                      keep_default=False)

        # Adding field 'Photo.title'
        db.add_column(u'viz_photo', 'title',
                      self.gf('django.db.models.fields.TextField')(null=True),
                      keep_default=False)

        # Adding field 'Photo.photoURL'
        db.add_column(u'viz_photo', 'photoURL',
                      self.gf('django.db.models.fields.TextField')(default=' '),
                      keep_default=False)

        # Adding field 'Photo.mainImage'
        db.add_column(u'viz_photo', 'mainImage',
                      self.gf('django.db.models.fields.BooleanField')(default=False),
                      keep_default=False)

        # Deleting field 'Photo.file'
        db.delete_column(u'viz_photo', 'file')

        # Adding field 'Viz.binaryURL'
        db.add_column(u'viz_viz', 'binaryURL',
                      self.gf('django.db.models.fields.TextField')(default=' '),
                      keep_default=False)

        # Deleting field 'Binary.file'
        db.delete_column(u'viz_binary', 'file')


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
            'path': ('django.db.models.fields.TextField', [], {}),
            'viz': ('django.db.models.fields.related.ForeignKey', [], {'to': u"orm['viz.Viz']"})
        },
        u'viz.photo': {
            'Meta': {'object_name': 'Photo'},
            'file': ('django.db.models.fields.files.FileField', [], {'max_length': '100'}),
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
            'sourceURL': ('django.db.models.fields.TextField', [], {})
        }
    }

    complete_apps = ['viz']