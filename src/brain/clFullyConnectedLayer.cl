__kernel void compute(__global float* excitations, unsigned int excitationCount, __global float* allNeuralWeights, __global float* activations){
 unsigned int neuralIndex = get_global_id(0);

 unsigned int offset = neuralIndex*excitationCount;
 float myExcitation = 0;
 for(unsigned int i=0; i < excitationCount; i++){
 	myExcitation += excitations[i] * allNeuralWeights[offset + i];
 }
 activations[neuralIndex] = tanh(myExcitation);
}