/* Tarea 1. Updatable Priority Queue
        Programming and Algorithms 2. CIMAT
            José Luis de León Carmona */

#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <sstream>

template <class TPriority, class TKey>
class UpdatableHeap {
private:
    struct Node {
        TPriority priority;
        TKey key;
    };

    std::vector<Node> heap;
    std::unordered_map<TKey, size_t> key_positions;

    void heapifyUp(size_t index) {
        while (index > 0) {
            size_t parent = (index - 1) / 2;
            if (heap[index].priority > heap[parent].priority ||
                (heap[index].priority == heap[parent].priority && heap[index].key >= heap[parent].key)) {
                std::swap(heap[index], heap[parent]);
                key_positions[heap[index].key] = index;
                key_positions[heap[parent].key] = parent;
                index = parent;
            } else {
                break;
            }
        }
    }

    void heapifyDown(size_t index) {
        size_t left_child, right_child, largest_child;

        while (true) {
            left_child = 2 * index + 1;
            right_child = 2 * index + 2;
            largest_child = index;

            if (left_child < heap.size() &&
                (heap[left_child].priority > heap[largest_child].priority ||
                 (heap[left_child].priority == heap[largest_child].priority &&
                  heap[left_child].key > heap[largest_child].key))) {
                largest_child = left_child;
            }

            if (right_child < heap.size() &&
                (heap[right_child].priority > heap[largest_child].priority ||
                 (heap[right_child].priority == heap[largest_child].priority &&
                  heap[right_child].key > heap[largest_child].key))) {
                largest_child = right_child;
            }

            if (largest_child != index) {
                std::swap(heap[index], heap[largest_child]);
                key_positions[heap[index].key] = index;
                key_positions[heap[largest_child].key] = largest_child;
                index = largest_child;
            } else {
                break;
            }
        }
    }

public:
    UpdatableHeap() = default;

    bool isInserted(const TKey &k) const {
        return key_positions.find(k) != key_positions.end();
    }

    void insertOrUpdate(const TPriority &p, const TKey &k) {
        if (isInserted(k)) {
            // Update existing node
            size_t index = key_positions[k];
            heap[index].priority = p;
            heapifyUp(index);
            heapifyDown(index);
        } else {
            // Insert new node
            heap.emplace_back(Node{p, k});
            size_t index = heap.size() - 1;
            key_positions[k] = index;
            heapifyUp(index);
        }
    }

    std::pair<TPriority, TKey> top() const {
        return heap.empty() ? std::make_pair(TPriority(-1), TKey()) : std::make_pair(heap[0].priority, heap[0].key);
    }

    void pop() {
        if (!heap.empty()) {
            size_t last_index = heap.size() - 1;
            key_positions.erase(heap[0].key);
            if (last_index > 0) {
                key_positions[heap[last_index].key] = 0;
                std::swap(heap[0], heap[last_index]);
            }
            heap.pop_back();
            if (!heap.empty()) {
                heapifyDown(0);
            }
        }
    }

    int getSize() const {
        return static_cast<int>(heap.size());
    }

    void erase(const TKey &k) {
        if (isInserted(k)) {
            size_t index = key_positions[k];
            size_t last_index = heap.size() - 1;
            key_positions.erase(k);

            if (index != last_index) {
                key_positions[heap[last_index].key] = index;
                std::swap(heap[index], heap[last_index]);
                heap.pop_back();
                if (index < heap.size()) {
                    heapifyUp(index);
                    heapifyDown(index);
                }
            } else {
                heap.pop_back();
            }
        }
    }
};

int main() {
    int OP;

    std::cin >> OP;

    std::cin.ignore();

    UpdatableHeap<int, std::string> alimentos;

    std::string linea, operacion, alimento;
    int valor;

    for (int i = 0; i < OP; i++) {

        std::getline(std::cin, linea);

        std::istringstream ss(linea);

        ss >> operacion >> alimento >> valor;

        if (operacion == "P") {
            auto topItem = alimentos.top();
            if (topItem.first != -1) {
                std::cout << topItem.second << " " << topItem.first << std::endl;
            } else {
                std::cout << topItem.first << std::endl;
            }
        }

        if (operacion == "IU") {
            alimentos.insertOrUpdate(valor, alimento);
        }

        if (operacion == "DL") {
            alimentos.pop();
        }

        if (operacion == "D") {
            alimentos.erase(alimento);
        }
    }

    return 0;
}