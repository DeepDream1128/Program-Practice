#include <iostream>
#include <string>
#include <ctime>

class Car {
public:
    std::string plate_number;
    int arrival_time;
    int departure_time;

    Car(std::string plate, int arrival) : plate_number(plate), arrival_time(arrival), departure_time(0) {}
};

class Stack {
    int top;
    int capacity;
    Car **cars;

public:
    Stack(int cap) : top(-1), capacity(cap) {
        cars = new Car *[capacity];
    }

    ~Stack() {
        for (int i = 0; i <= top; i++) {
            delete cars[i];
        }
        delete[] cars;
    }

    bool push(Car *car) {
        if (top + 1 >= capacity) {
            return false;
        }
        cars[++top] = car;
        return true;
    }

    Car *pop() {
        if (top == -1) {
            return nullptr;
        }
        return cars[top--];
    }

    int size() {
        return top + 1;
    }

    bool isEmpty() {
        return top == -1;
    }

    Car *getTop() {
        return cars[top];
    }
};

class QueueNode {
public:
    Car *car;
    QueueNode *next;

    QueueNode(Car *car) : car(car), next(nullptr) {}
};

class Queue {
    QueueNode *front;
    QueueNode *rear;

public:
    Queue() : front(nullptr), rear(nullptr) {}

    ~Queue() {
        while (front != nullptr) {
            QueueNode *temp = front;
            front = front->next;
            delete temp->car;
            delete temp;
        }
    }

    bool isEmpty() {
        return front == nullptr;
    }

    void enqueue(Car *car) {
        QueueNode *new_node = new QueueNode(car);
        if (rear == nullptr) {
            front = rear = new_node;
            return;
        }
        rear->next = new_node;
        rear = new_node;
    }

    Car *dequeue() {
        if (isEmpty()) {
            return nullptr;
        }
        QueueNode *temp = front;
        Car *car = temp->car;
        front = front->next;
        if (front == nullptr) {
            rear = nullptr;
        }
        delete temp;
        return car;
    }
};

int main() {
    int parking_capacity = 5;
    int fee_per_minute = 1;
    Stack parking_lot(parking_capacity);
    Queue waiting_lane;

    std::string action, plate_number;
    int timestamp;
    std::cout<<"请输入操作："<<std::endl;
    while (std::cin >> action >> plate_number >> timestamp) {
        if (action == "到达") {
            Car *car = new Car(plate_number, timestamp);
            if (!parking_lot.push(car)) {
                waiting_lane.enqueue(car);
                std::cout << "车辆 " << plate_number << " 在便道停车" << std::endl;
            } else {
                std::cout << "车辆 " << plate_number << " 在停车场停车" << std::endl;
            }
        } else if (action == "离去") {
            Stack temp_stack(parking_capacity);
            bool car_found = false;
            while (!parking_lot.isEmpty()) {
                Car *current_car = parking_lot.pop();
                if (current_car->plate_number == plate_number) {
                   
current_car->departure_time = timestamp;
int stay_duration = current_car->departure_time - current_car->arrival_time;
int fee = stay_duration * fee_per_minute;
std::cout << "车辆 " << plate_number << " 离开停车场, 停留时间：" << stay_duration << " 分钟, 应缴纳费用： " << fee << std::endl;
car_found = true;
delete current_car;
break;
} else {
temp_stack.push(current_car);
}
}
        if (!car_found) {
            std::cout << "未找到车辆 " << plate_number << std::endl;
        } else {
            while (!temp_stack.isEmpty()) {
                parking_lot.push(temp_stack.pop());
            }

            if (!waiting_lane.isEmpty()) {
                Car *car_to_enter = waiting_lane.dequeue();
                parking_lot.push(car_to_enter);
                std::cout << "车辆 " << car_to_enter->plate_number << " 从便道进入停车场" << std::endl;
            }
        }
    } else {
        std::cout << "无效操作" << std::endl;
    }
}

return 0;
}