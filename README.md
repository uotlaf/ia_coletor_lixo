# ROBÔ COLETOR DE LIXO

## Regras:
### Ambiente
- Gerar X lixos orgânicos e X lixos recicláveis
- Lixo orgânicos valem +1 e recicláveis valem +5

### Robô
- O robô tem preferência por coletar lixo reciclável
- O robô deve carregar apenas um lixo por vez
- O robô pode executar as seguintes ações: **Esquerda**, **Direita**, **Cima**, **Baixo**, **Pegar**, **Soltar** e **NoOp**
- O robô tem as seguintes percepções: **Local onde está**, **Conteúdo do local**, **Conteúdo dos 8 locais vizinhos** e **local da lixeira**
- Devem ser implementados 4 agentes: **Reativo simples**, **baseado em modelos**, **objetivos** e **utilidade**
- Deve-se medir o tempo de execução de cada uma das arquiteturas para avaliar o que teve o melhor desempenho

## Informações gerais
Linguagem: C17 \
Ambiente: Linux/gcc \
Arquivos fonte: Todos em [./src](./src) \
Cabeçalhos: Todos em [./include](./include)

Alunos: [Uotlaf](https://github.com/uotlaf/) e [Gabriel Silva](https://github.com/gabrielsilva28)