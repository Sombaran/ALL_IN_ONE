import re
import ast
from setuptools import setup

with open('README.rst') as f:
    readme = f.read()
with open('LICENSE') as f:
    license = f.read()

setup(
    name='mobile_agent',
    version='1.1',
    description='mobile_agent',
    long_description=readme,
    author='Praveen kumar donga',
    author_email='praveen.donga@smartron.com',
    url='',
    license=license,
    packages=['mobile_agent', 'mobile_agent.ext']
)

