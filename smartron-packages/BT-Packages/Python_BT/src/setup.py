
#__author__ = "Adarsh B U and Harish Gaddameedi"
#__porting__ = "Adarsh B U and Praneet Kaur"
#__copyright__ = "Copyright 2017 - 2018, Smartron Pvt. Ltd."
#__credits__ = ["Adarsh BU, Praneet Kaur and Harish Gaddameedi"]
#__maintainer__ = "adarsh BU"
#__email__ = "adarsh.umesh@smartron.com"

from setuptools import setup

with open('README.md') as f:
    readme = f.read()
    
with open('LICENSE') as f:
    license = f.read()
    
setup(
    name='python_BT',
    version='0.1.0',
    description='BT_Module',
    long_description=readme,
    author='Harish, Adarsh, Praneet',
    author_email='adarsh.umesh@smartron.com',
    url='',
    license=license,
    packages=['python_BT', 'python_BT.ext']
)

