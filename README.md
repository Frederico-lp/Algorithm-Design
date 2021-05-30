# calProject

## À Procura de Estacionamento

### Requisitos

- SFML multimedia library
- CLion

### Instruções

- Dowload do repositório com o projeto;
- Abrir a pasta calProject no Clion;
- No ficherio CMakeLists.txt, clickar no botão direito do rato e fazer "Load CMake Project";
- Build 
- Run 

## Utilização

### Origem e Destino

- Input "Node id of the vertex you want to start on" -> Ponto de Origem;
- Input "Node id of the vertex you want to finish on" -> Ponto de Destino;

### Opções de procura de parque

##### Opção 1

- Parque mais próximo da origem(caso selecione destinos intermédios será o parque mais próximo do ultimo destino intermédio visitado)

##### Opção 2

- Parque mais barato(em caso de empate de preço o algoritmo seleciona o parque mais próximo da origem, ou do último ponto intermédio visitado caso existam)

##### Opção 3

- Parque mais próximo do ponto de destino(no caso de haver destinos intermédios, o último a ser visitado será o mais próximo possivel do parque selecionado)

### Algoritmos

- Dijkstra (Recomendado dada a dimensão do data set) - Opção 1 
- BellmanFord - Opção 0 

### Destinos Intermédios

- Opção 0 -> Ir direto ao parque de estacionamento;
- Opção 1 -> Visitar somente uma farmácia;
- Opção 3 -> Visitar somente uma bomba de gasolina;
- Opção 4 -> Visitar uma farmácia e uma bomba de gasolina;
- Opção 5 -> Visitar somente um café;
- Opção 6 -> Visitar uma farmácia e um café; 
- Opção 8 -> Visitar uma bomba de gasolina e um café;
- Opção 9 -> Visitar uma farmácia,uma bomba de gasolina e um café;

(Os trajetos com mais de um destino intermédio não obrigam a que ordem seja a acima apresentada. O algoritmo tentará para cada opção de parque de estacionamento encontrar a melhor ordem de visita aos destinos intermédios)


