#include <graph.h>

void Graph::process_input() {
    if (!commandQueue.empty()) {
        if (commandQueue.front() == "quit") {
            glfwSetWindowShouldClose(window, true);
        }

        else if (commandQueue.front() == "add" && commandQueue.size() == 3) {
            commandQueue.pop();
            std::string name = commandQueue.front();
            commandQueue.pop();
            std::string infix = commandQueue.front();
            commandQueue.pop();

            nameSet.insert(name);
            nameToInfix[name] = infix;
            nameToExpTree[name] = gen_exp_tree(name);

            gen_graph_array_buffer();

            inputCV.notify_one();
        }
    }
}
