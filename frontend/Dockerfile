FROM gcc:latest

WORKDIR /app

COPY frontend ./frontend

WORKDIR /app/frontend/backend

RUN g++ main.cpp -o backend -pthread

EXPOSE 8080

CMD ["./backend"]