import re
import ast
from setuptools import setup

with open('README.rst') as f:
    readme = f.read()
with open('LICENSE') as f:
    license = f.read()

setup(
    name='web_socket',
    version='1.3',
    description='Web Socket python library',
    long_description='Web Socket package to manage IOT Commands on Smartron IOT Hub - This is private property of '
                     'Smartron Pvt. Ltd. and hold Copyright excluding any other third part open source library such '
                     'as pyzmq, request, SocketIO_Client',
    classifiers=[
        'Intended Audience :: Developers',
        'Programming Language :: Python',
        'Development Status :: 4 - Beta',
    ],
    keywords='Web Socket',
    author='Subhash kumar and Hemant Jiwani',
    author_email=['subhash.kumar@smartron.com', 'hemant.jiwani@smartron.com'],
    install_requires=[
    ],
    tests_require=[
        'nose',
        'coverage',
    ],
    packages=['web_socket', 'web_socket.ext']
)
