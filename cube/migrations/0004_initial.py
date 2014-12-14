# -*- coding: utf-8 -*-
import datetime
from south.db import db
from south.v2 import SchemaMigration
from django.db import models


class Migration(SchemaMigration):

    def forwards(self, orm):
        # Adding model 'CubeUser'
        db.create_table(u'cube_cubeuser', (
            (u'id', self.gf('django.db.models.fields.AutoField')(primary_key=True)),
            ('user', self.gf('django.db.models.fields.TextField')()),
            ('accessToken', self.gf('django.db.models.fields.TextField')()),
        ))
        db.send_create_signal(u'cube', ['CubeUser'])

        # Adding model 'Cube'
        db.create_table(u'cube_cube', (
            (u'id', self.gf('django.db.models.fields.AutoField')(primary_key=True)),
            ('coreName', self.gf('django.db.models.fields.TextField')()),
            ('coreID', self.gf('django.db.models.fields.TextField')()),
            ('firmware', self.gf('django.db.models.fields.TextField')()),
        ))
        db.send_create_signal(u'cube', ['Cube'])


    def backwards(self, orm):
        # Deleting model 'CubeUser'
        db.delete_table(u'cube_cubeuser')

        # Deleting model 'Cube'
        db.delete_table(u'cube_cube')


    models = {
        u'cube.cube': {
            'Meta': {'object_name': 'Cube'},
            'coreID': ('django.db.models.fields.TextField', [], {}),
            'coreName': ('django.db.models.fields.TextField', [], {}),
            'firmware': ('django.db.models.fields.TextField', [], {}),
            u'id': ('django.db.models.fields.AutoField', [], {'primary_key': 'True'})
        },
        u'cube.cubeuser': {
            'Meta': {'object_name': 'CubeUser'},
            'accessToken': ('django.db.models.fields.TextField', [], {}),
            u'id': ('django.db.models.fields.AutoField', [], {'primary_key': 'True'}),
            'user': ('django.db.models.fields.TextField', [], {})
        }
    }

    complete_apps = ['cube']