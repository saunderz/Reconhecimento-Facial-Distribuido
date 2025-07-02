# 🛡️ Sistema Distribuído de Reconhecimento Facial para Controle de Acesso

Este projeto implementa um sistema distribuído para autenticação de usuários por reconhecimento facial, aplicando conceitos de **redes neurais profundas**, **edge computing**, **protocolos eficientes de comunicação** e **middleware inteligente** para promover segurança, escalabilidade e interoperabilidade em ambientes multiusuário.

---

## 🚀 **Visão Geral**

- **Arquitetura:** Cliente-Servidor distribuído com integração ao middleware **InterSCity**.
- **Cliente:** Dispositivo embarcado (ex: ESP32-S3) ou câmera conectada a um nó de captura.
- **Servidor:** Processa as imagens, realiza reconhecimento facial com modelos CNNs e decide pela liberação ou negação do acesso.
- **Comunicação:** Socket TCP ou gRPC para transmissão de dados, MQTT para integração IoT.
- **Interface:** Dashboard web para cadastro de usuários, gerenciamento de permissões e visualização de logs de acesso.

---

## 🔧 **Tecnologias Utilizadas**

| **Tecnologia**              | **Descrição**                                                                                             |
|------------------------------|-----------------------------------------------------------------------------------------------------------|
| **TensorFlow Lite / MediaPipe** | Frameworks de inferência otimizados para execução embarcada (edge computing).                        |
| **OpenCV / Dlib / face_recognition** | Bibliotecas para detecção, alinhamento e reconhecimento facial.                               |
| **MQTT / gRPC**             | Protocolos leves e eficientes para comunicação assíncrona entre nós e serviços.                           |
| **InterSCity**              | Middleware open-source que gerencia dispositivos distribuídos como recursos RESTful interoperáveis.        |
| **ESP32-S3**                | Microcontrolador com suporte nativo a câmeras DVP, PSRAM e aceleração SIMD para IA embarcada.             |
| **TLS 1.3 / RBAC**          | Criptografia ponta-a-ponta e controle de acesso baseado em papéis, garantindo segurança dos dados.         |

---

## 📂 **Estrutura do Repositório**

```
/cliente/
  └── captura_envio.py        # Código do cliente (ESP32-S3/Raspberry) para captura e envio da imagem
/servidor/
  ├── servidor_main.py        # Backend com processamento de reconhecimento facial
  ├── models/                 # Modelos CNN treinados (FaceNet/ArcFace)
  └── database/               # Scripts e schemas do banco de dados de usuários e logs
dashboard/
  └── app.py                  # Interface web para administração
docs/
  └── arquitetura.md          # Documentação detalhada do sistema
README.md
requirements.txt
```

---

## ⚙️ **Instalação e Execução**

### 🔹 **1. Clone o repositório**

```bash
git clone https://github.com/SeuUsuario/sistema-reconhecimento-facial-distribuido.git
cd sistema-reconhecimento-facial-distribuido
```

### 🔹 **2. Crie e ative o ambiente virtual**

```bash
python -m venv venv
source venv/bin/activate  # Linux/Mac
venv\Scripts\activate     # Windows
```

### 🔹 **3. Instale as dependências**

```bash
pip install -r requirements.txt
```

### 🔹 **4. Execute o servidor**

```bash
cd servidor
python servidor_main.py
```

### 🔹 **5. Execute o cliente (nó de captura)**

```bash
cd cliente
python captura_envio.py
```

### 🔹 **6. Acesse o dashboard**

- Abra o navegador em [http://localhost:5000](http://localhost:5000)

---

## 📝 **Funcionalidades**

- Captura de imagens faciais no nó cliente.
- Envio de imagens via **socket TCP/gRPC** para o servidor central.
- Processamento facial com modelos **FaceNet / ArcFace**.
- Autorização ou negação do acesso em tempo real.
- Registro de logs de acesso com timestamps, status e usuário.
- Dashboard web para cadastro e gerenciamento de permissões.
- Integração com **InterSCity** para gestão como recurso urbano inteligente.

---

## 🔒 **Segurança**

- Criptografia de comunicação com **TLS 1.3**.
- Controle de acesso com **RBAC** para diferentes níveis de usuário.
- Armazenamento seguro de embeddings faciais no banco de dados.

---

## 🧠 **Possíveis Evoluções**

- Integração com **biometria multivariada** (face + voz + impressão digital).
- Implementação de **reconhecimento facial 3D** para maior robustez.
- Uso de **Federated Learning** para treinamento distribuído sem compartilhamento de dados brutos.
- Auditoria de acessos com **blockchain** para logs imutáveis.

---

## 👥 **Autores**

- **Luã Saunders** – Desenvolvimento do backend, visão computacional e integração embarcada.
- **Equipe** – Colaboradores do projeto.
  

---

## 📄 **Licença**

Este projeto está licenciado sob a [MIT License](LICENSE).

---

## 📞 **Contato**

Para dúvidas ou colaboração, entre em contato: [saunders.luan@gmail.com]

---

```

