# Generated by Django 4.0.5 on 2022-06-08 14:33

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('pbl', '0001_initial'),
    ]

    operations = [
        migrations.CreateModel(
            name='Device',
            fields=[
                ('id', models.BigAutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('name', models.CharField(max_length=255)),
                ('last_update', models.CharField(default='', max_length=255)),
            ],
        ),
        migrations.RemoveField(
            model_name='siteuser',
            name='created_at',
        ),
        migrations.RemoveField(
            model_name='siteuser',
            name='is_editor',
        ),
        migrations.RemoveField(
            model_name='siteuser',
            name='subscriptions',
        ),
        migrations.AddField(
            model_name='siteuser',
            name='devices',
            field=models.ManyToManyField(related_name='users', to='pbl.device'),
        ),
    ]
