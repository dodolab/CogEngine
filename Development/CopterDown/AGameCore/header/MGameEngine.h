
#ifndef GAMEENGINE_H
#define GAMEENGINE_H

void Update(uint64 delta, uint64 absolute);
void Draw(uint64 delta, uint64 absolute);

void StartLoop();

void Init();
void Terminate();



#endif
