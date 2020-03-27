#!/usr/bin/env python
import json
import time
import uuid
import os
import subprocess

from compilation import files
from flask import Flask
from flask import request
from flask_cors import CORS, cross_origin


app = Flask('backend')
cors = CORS(app)
app.config['CORS_HEADERS'] = 'Content-Type'


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

    run_test(name)

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
    process = subprocess.run(
        f'cd /tmp && ./{executable}',
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        shell=True
    )


class CompilationError(RuntimeError):
    pass


app.run(host='0.0.0.0', port=3322)
