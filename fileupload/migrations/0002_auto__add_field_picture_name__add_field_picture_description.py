# -*- coding: utf-8 -*-
import datetime
from south.db import db
from south.v2 import SchemaMigration
from django.db import models


class Migration(SchemaMigration):

    def forwards(self, orm):
        # Adding field 'Picture.name'
        db.add_column(u'fileupload_picture', 'name',
                      self.gf('django.db.models.fields.TextField')(default=' '),
                      keep_default=False)

        # Adding field 'Picture.description'
        db.add_column(u'fileupload_picture', 'description',
                      self.gf('django.db.models.fields.TextField')(default='', blank=True),
                      keep_default=False)


    def backwards(self, orm):
        # Deleting field 'Picture.name'
        db.delete_column(u'fileupload_picture', 'name')

        # Deleting field 'Picture.description'
        db.delete_column(u'fileupload_picture', 'description')


    models = {
        u'fileupload.picture': {
            'Meta': {'object_name': 'Picture'},
            'description': ('django.db.models.fields.TextField', [], {'blank': 'True'}),
            'file': ('django.db.models.fields.files.ImageField', [], {'max_length': '100'}),
            u'id': ('django.db.models.fields.AutoField', [], {'primary_key': 'True'}),
            'name': ('django.db.models.fields.TextField', [], {})
        }
    }

    complete_apps = ['fileupload']