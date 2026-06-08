# 🌍 UrbanHeat Edge Station

## Global Solution 2026 – FIAP

### Disciplina: Edge Computing

---

# 📖 Descrição do Projeto

O UrbanHeat Edge Station é uma estação inteligente de monitoramento ambiental desenvolvida como complemento ao projeto UrbanHeat. A solução utiliza sensores embarcados para coletar dados ambientais em tempo real e identificar situações associadas às ilhas de calor urbanas.

A proposta está conectada à Indústria Espacial por utilizar os mesmos conceitos de monitoramento ambiental empregados por satélites de observação da Terra. Enquanto os satélites realizam análises em larga escala, a estação Edge atua localmente, permitindo respostas rápidas e monitoramento contínuo das condições ambientais.

---

# 🎯 Objetivo da Solução

Desenvolver um sistema embarcado capaz de monitorar temperatura e umidade do ambiente, calcular níveis de prioridade ambiental e emitir alertas visuais e sonoros para auxiliar na identificação de regiões com potencial risco associado às ilhas de calor urbanas.

---

# 🔧 Componentes Utilizados

| Componente           | Quantidade |
| -------------------- | ---------- |
| Arduino Uno          | 1          |
| Sensor DHT22         | 1          |
| Display LCD I2C 16x2 | 1          |
| LED Verde            | 1          |
| LED Amarelo          | 1          |
| LED Vermelho         | 1          |
| Buzzer               | 1          |
| Resistores 220Ω      | 3          |
| Jumpers              | Diversos   |

---

# ⚙️ Funcionamento do Sistema

O sistema realiza leituras periódicas dos dados ambientais através do sensor DHT22.

As informações coletadas são processadas localmente pelo Arduino, aplicando uma lógica de classificação baseada em temperatura e umidade.

O resultado é exibido no display LCD e representado visualmente através dos LEDs.

### Classificações

#### 🟢 Prioridade Baixa

* Temperatura abaixo de 30°C
* LED verde ligado
* Ambiente considerado estável

#### 🟡 Prioridade Média

* Temperatura entre 30°C e 35°C
* LED amarelo ligado
* Situação de atenção

#### 🔴 Prioridade Alta

* Temperatura acima de 35°C
* LED vermelho ligado
* Alerta sonoro acionado pelo buzzer

### Influência da Umidade

O sistema também considera a umidade relativa do ar.

Quando a umidade é elevada, o índice de risco ambiental recebe uma penalização adicional, tornando a classificação mais sensível às condições climáticas reais.

---

# 🔌 Estrutura do Circuito

| Componente   | Pino Arduino |
| ------------ | ------------ |
| DHT22 Data   | D2           |
| LED Verde    | D8           |
| LED Amarelo  | D9           |
| LED Vermelho | D10          |
| Buzzer       | D11          |
| LCD SDA      | A4           |
| LCD SCL      | A5           |

---


# 🛰️ Relação com a Indústria Espacial

O UrbanHeat foi inspirado em sistemas de monitoramento ambiental que utilizam dados de satélites para identificar áreas urbanas sujeitas a ilhas de calor.

A estação Edge desenvolvida neste projeto atua como uma camada complementar de monitoramento local, permitindo análises em tempo real diretamente na região monitorada.

Essa integração entre sensoriamento remoto e processamento local representa uma aplicação prática dos conceitos utilizados em cidades inteligentes e sustentabilidade urbana.

---

# 👥 Integrantes

Octávio Mello Covre de Souza — RM: 571811

Gabriel Torres Zambo — RM: 569883

Hiago Silva de Oliveira — RM: 572664

Juliano Galhardo de Oliveira — RM: 572493

---

# 🔗 Simulação no Wokwi

A simulação completa do projeto pode ser acessada através do link abaixo:

👉 https://wokwi.com/projects/466113058335723521

O ambiente permite visualizar o circuito, alterar os valores do sensor DHT22 e acompanhar em tempo real o comportamento do sistema, incluindo os alertas visuais e sonoros, a classificação de prioridade ambiental e a exibição das informações no display LCD.

---

# 🎥 Vídeo Demonstrativo

O vídeo de apresentação da solução foi desenvolvido para a disciplina de Storytelling e Inspiração Empreendedora e demonstra o funcionamento completo do projeto UrbanHeat.

---

# ✅ Status do Projeto

Projeto desenvolvido para a Global Solution 2026 da FIAP como aplicação de conceitos de Edge Computing, Internet das Coisas (IoT), sustentabilidade urbana e tecnologias inspiradas na Indústria Espacial.
