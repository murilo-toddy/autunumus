FROM python:3.11

WORKDIR /app

COPY ../../requirements.txt .
RUN pip install --no-cache-dir -r requirements.txt

COPY ../../zmq_source .

CMD ["python3", "./source.py"]
