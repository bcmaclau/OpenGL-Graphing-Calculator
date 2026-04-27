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
            std::cout << name << "(0) = " << eval_exp_tree(nameToExpTree[name], 0.0f) << std::endl;

            regenArrayBuffer = true;

            switch (colorCycle) {
                case 0:
                    nameToColor[name] = glm::vec3(1.0f, 0.0f, 0.0f);
                    colorCycle++;
                    break;
                case 1:
                    nameToColor[name] = glm::vec3(0.0f, 0.0f, 1.0f);
                    colorCycle++;
                    break;
                case 2:
                    nameToColor[name] = glm::vec3(0.0f, 1.0f, 0.0f);
                    colorCycle++;
                    break;
                case 3:
                    nameToColor[name] = glm::vec3(1.0f, 0.5f, 0.0f);
                    colorCycle++;
                    break;
                case 4:
                    nameToColor[name] = glm::vec3(0.5f, 0.0f, 0.5f);
                    colorCycle++;
                    break;
                default:
                    nameToColor[name] = glm::vec3(0.0f, 0.0f, 0.0f);
                    colorCycle = 0;
                    break;
            }

            inputCV.notify_one();
        }

        else if (commandQueue.front() == "remove" && commandQueue.size() == 2) {
            commandQueue.pop();

            std::string name = commandQueue.front();
            commandQueue.pop();

            nameSet.erase(name);
            nameToInfix.erase(name);
            nameToColor.erase(name);
            nameToExpTree.erase(name);

            regenArrayBuffer = true;

            inputCV.notify_one();
        }

        else if (commandQueue.front() == "list") {
            commandQueue.pop();

            if (nameSet.size() == 0) {
                std::cout << "No functions entered yet." << std::endl;
            }
            else {
                std::cout << "List of all functions:" << std::endl;
                for (const auto& pair : nameToInfix) {
                    std::cout << pair.first << "(x) = " << pair.second << std::endl;
                }
            }

            inputCV.notify_one();
        }
    }
}
