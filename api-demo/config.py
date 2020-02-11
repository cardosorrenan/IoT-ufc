import os.path

basedir = os.path.abspath(os.path.dirname(__file__))
path = os.path.join(basedir, 'storage.db')
SQLALCHEMY_DATABASE_URI = 'sqlite:///' + path
SQLALCHEMY_TRACK_MODIFICATIONS = True
DEBUG = True