FROM python:3.6

COPY . /app

WORKDIR /app/docs/backend

RUN pip3 install -r requirements.txt

ENV PYTHONPATH=/app/framework

ENTRYPOINT ["python3"]

CMD ["backend.py"]
