# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations


class Migration(migrations.Migration):

    dependencies = [
        ('viz', '0002_auto_20150505_2259'),
    ]

    operations = [
        migrations.AddField(
            model_name='viz',
            name='published',
            field=models.BooleanField(default=False),
        ),
    ]
