FROM ubuntu:latest

RUN mkdir /app
COPY signal /app/
WORKDIR /app

ENTRYPOINT ["./signal"]