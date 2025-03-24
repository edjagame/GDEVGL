// Tails Matrix Transformations
    // Time 0: idle time
    // Time 1: rotating time
    // Time 2: Movement time
    std::vector<std::pair<int, int>> tailsValidMoves;
    std::vector<float> tailsValidAngles;
    int newRow, newColumn;
    float tailsTime0 = 0.3f, tailsTime1 = 1.0f, tailsTime2 = 2.0f;
    
    // Main loop that calculates the transformations and passes them to the vertex array
    for (int i = 0; i < 2; i++) {
        tailsInstances[i].t += deltaTime;
        tailsValidMoves.clear();
        tailsValidAngles.clear();

        for(unsigned int j=0; j<knightMoves.size(); j++) {
            newRow = tailsInstances[i].row + knightMoves[j].first;
            newColumn = tailsInstances[i].column + knightMoves[j].second;
            if(newRow >= 0 && newRow < 8 && newColumn >= 0 && newColumn < 8) {
                tailsValidMoves.push_back({knightMoves[j].first, knightMoves[j].second});
                tailsValidAngles.push_back(knightAngles[j]);
            }
        }

        // Calculate the next move in the idle state
        if (tailsInstances[i].state == 0){
            if (tailsInstances[i].t >= tailsTime0) {
                tailsInstances[i].nextMove = rand() % tailsValidMoves.size();
                tailsInstances[i].state = 1;
                tailsInstances[i].t = 0;
            }
        }

        // ... set up the model matrix...
        glm::mat4 modelTransform = glm::mat4(1.0f);  // set to identity first
        
        // Set the initial position of the model
        modelTransform = glm::translate(modelTransform,
                                    glm::vec3(tailsInstances[i].x, tailsInstances[i].y, tailsInstances[i].z)); // translate xyz
        
        // Translate it to a different square in the movement phase, then bring it back to idle state
        if (tailsInstances[i].state == 2) {
            float timeScaling = tailsInstances[i].t / tailsTime2;
            float translateX = tailsValidMoves[tailsInstances[i].nextMove].second * tileWidth * timeScaling;
            float translateY = 5 * sin(PI * timeScaling);
            float translateZ = tailsValidMoves[tailsInstances[i].nextMove].first * tileWidth * timeScaling;
            modelTransform = glm::translate(modelTransform, glm::vec3(translateX, translateY, translateZ));
            if (tailsInstances[i].t >= tailsTime2) {
                tailsInstances[i].state = 0;
                tailsInstances[i].t = 0;
                tailsInstances[i].row += tailsValidMoves[tailsInstances[i].nextMove].first;
                tailsInstances[i].column += tailsValidMoves[tailsInstances[i].nextMove].second;
                tailsInstances[i].x = originX + offsetX + tailsInstances[i].column * tileWidth;
                tailsInstances[i].z = originZ + offsetZ + tailsInstances[i].row * tileWidth;
            } 
        }

        // Set the initial rotation
        modelTransform = glm::rotate(modelTransform,
                                    glm::radians(tailsInstances[i].rotation),
                                    glm::vec3(0.0f, 1.0f, 0.0f));

        // Rotate it according to the next move in the rotating phase
        if (tailsInstances[i].state == 1) {
            float turnAngle = tailsValidAngles[tailsInstances[i].nextMove] - tailsInstances[i].rotation - 90.0f;
            if(fabs(turnAngle) > 180) {
                turnAngle = 360 - fabs(turnAngle);
            }
            float timeScaling = (tailsInstances[i].t / tailsTime1);
            modelTransform = glm::rotate(modelTransform, 
                                         glm::radians(turnAngle * timeScaling),
                                         glm::vec3(0.0f, 1.0f, 0.0f));

            if (tailsInstances[i].t >= tailsTime1) {
                tailsInstances[i].state = 2;
                tailsInstances[i].rotation = tailsValidAngles[tailsInstances[i].nextMove] - 90;
                tailsInstances[i].t = 0;    
            }
        }

        // Rotation during the movement phase
        if(tailsInstances[i].state == 2) {
            modelTransform = glm::rotate(modelTransform,
                                        glm::radians(0.0f * tailsInstances[i].t),
                                        glm::vec3(0.0f, 0.0f, 1.0f));
        }

        

        //Scale (in case its needed)
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(tailsInstances[i].scaling, tailsInstances[i].scaling, tailsInstances[i].scaling));   // scale x and y
        
        modelTransform = glm::mat4(1.0f);
        glUniformMatrix4fv(glGetUniformLocation(shader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));