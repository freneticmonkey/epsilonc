'''
Created on Feb 4, 2012

@author: scottporter
'''
from core.basesingleton import BaseSingleton

class Settings(BaseSingleton):

    def __init__(self):
        self.settings = {
            'ResourceSettings' : 
            {
                'path' : 'resources'
            }
        }

    @classmethod
    def get(cls, section, option):
        return cls.get_instance()._get(section, option)

    @classmethod
    def set(cls, section, option, value):
        cls.get_instance()._set(section, option, value)

    @classmethod
    def has_section(cls, section):
        return section in cls.get_instance().settings

    @classmethod
    def get_sections(cls):
        return cls.get_instance().settings.keys()

    @classmethod
    def has_option(cls, section, option):
        if Settings.has_section(section):
            return option in cls.get_instance().settings[section]
        print "Can't find section: " + section
        print cls.get_instance().settings
        return False

    @classmethod
    def get_options(cls, section):
        if Settings.has_section(section):
            return cls.get_instance().settings[section].keys()
        return []

    def _get(self, section, option):
        if section in self.settings:
            if option in self.settings[section]:
                return self.settings[section][option]
        return None

    def _set(self, section, option, value):
        if not section in self.settings:
            self.settings[section] = {}
        self.settings[section][option] = value
