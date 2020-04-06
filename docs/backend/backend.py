#!/usr/bin/env python
import json
import time
import uuid
import os
import sys
import subprocess
from subprocess import TimeoutExpired

from compilation import files
from flask import Flask
from flask import request
from flask_cors import CORS, cross_origin


app = Flask('backend')
cors = CORS(app)
app.config['CORS_HEADERS'] = 'Content-Type'
run_test_timeout = 0.1


@app.route('/test', methods=['POST'])
@cross_origin()
def run_test():
    source = request.data
    name = uuid.uuid1()
    path = f'/tmp/{name}.cpp'

    files.create_text_file(path, encode_source(source))

    try:
        compile(path)
    except CompilationError as err:
        return json.dumps({'problems': {'compilation': str(err)}}), 400

    try:
        run_test(name)
    except TimeoutExpired:
        return json.dumps({'problems': {'timed_out': run_test_timeout}}), 408

    result = files.read_file('/tmp/test_summary.jsonl')

    delete_files(name)

    return result, 200


def encode_source(source):
    return source.decode('utf-8') + '\n'


def delete_files(name):
    files.delete_file(f'/tmp/{name}.cpp')
    files.delete_file(f'/tmp/{name}')
    files.delete_file(f'/tmp/test_summary.jsonl')


def compile(path):
    executable = path.replace('.cpp', '')

    process = subprocess.run(
        f'g++ -std=c++11 -I../../framework -O0 {path} -o {executable}',
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        shell=True
    )

    if process.returncode != 0:
        raise CompilationError(process.stdout.decode('utf-8'))


def run_test(executable):
    process = subprocess.Popen(
        f'cd /tmp && ./{executable}',
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        shell=True
    )

    process.communicate(timeout=run_test_timeout)


class CompilationError(RuntimeError):
    pass

if os.getenv('UNSAFE'):
     app.run(host='0.0.0.0', port=3322)
     sys.exit(0)

certificate = os.getenv('CERT')
private_key = os.getenv('PRIVKEY')

if not certificate or not private_key:
    print('Failed to start backend: SSL keys not provided')
    sys.exit(1)

app.run(host='0.0.0.0', port=3322, ssl_context=(certificate, private_key))
