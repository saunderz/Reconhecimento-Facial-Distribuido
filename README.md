# 🛡️ Sistema Distribuído de Reconhecimento Facial para Controle de Acesso

Este projeto implementa um sistema distribuído para autenticação de usuários por reconhecimento facial, aplicando conceitos de **redes neurais profundas**, **edge computing**, **middleware inteligente** e **backend em C**, garantindo segurança, escalabilidade e interoperabilidade em ambientes multiusuário.

---

## 🚀 Visão Geral

- **Arquitetura:** Nós de captura embarcados se registram e enviam eventos de acesso ao middleware **InterSCity**, que intermedia e disponibiliza os serviços de reconhecimento e controle.
- **Cliente (ESP32-S3):** Captura imagem facial e publica eventos no **InterSCity Data Collector**.
- **Servidor (Backend em C):**
  - Consome eventos de captura via **InterSCity Resource Catalog** e **Data Collector** (HTTP REST).
  - Executa `recognize.py` para reconhecimento facial.
  - Persistência de logs em **SQLite**.
  - Publica resultados de autorização como eventos no **InterSCity Actuator Controller**.
- **Dashboard Web (Python):** Implementado em **Flask/FastAPI**, consome APIs do **InterSCity** para exibir logs, gerenciar usuários e visualizar status.

---

## 🔧 Tecnologias Utilizadas

| Tecnologia                        | Papel no Projeto                                                                                                       |
|-----------------------------------|------------------------------------------------------------------------------------------------------------------------|
| **C**                             | Desenvolvimento do backend que consome e publica eventos no InterSCity e persiste logs em SQLite.                     |
| **Python**                        | Script `recognize.py` para processamento de reconhecimento facial (OpenCV, face_recognition) e dashboard web.         |
| **FaceNet / ArcFace**             | Modelos CNN para geração de embeddings faciais robustos usados pelo script Python.                                     |
| **ESP32-S3**                      | Nó de captura embarcado que registra recurso e publica eventos de imagem no middleware InterSCity.                    |
| **InterSCity (HTTP REST)**        | Middleware que gerencia o ciclo completo: registro de recurso, ingestão de dados, e controle de atuadores.           |
| **SQLite**                        | Banco de dados local para armazenamento de logs de acesso quando necessário.                                           |
| **Flask / FastAPI**               | Framework para implementação do dashboard web, consumindo APIs REST do InterSCity para interface administrativa.       |

---

## 📂 Estrutura do Repositório

```
cliente/                         # Código do cliente embarcado (ESP32-S3)
  └── captura_envio.py           # Publica evento de captura de imagem no InterSCity
servidor/                        # Backend principal em C
  ├── backend_reconhecimento_facial.c  # Consome/publica eventos InterSCity, SQLite
  └── recognize.py              # Script Python de reconhecimento facial
dashboard/                       # Interface web em Python
  └── app.py                    # Dashboard com Flask ou FastAPI, consome APIs InterSCity
README.md                        # Documentação orientativa (este arquivo)
requirements.txt                 # Dependências Python
```

---

## ⚙️ Instalação e Execução

### 1. Preparar ambiente Python

```bash
python3 -m venv venv
source venv/bin/activate    # Linux/Mac
# Instalar dependências
pip install --upgrade pip
pip install -r requirements.txt
```

### 2. Compilar o Backend em C

```bash
gcc servidor/backend_reconhecimento_facial.c -o servidor/backend -lsqlite3 -lcurl
```

### 3. Configurar e Iniciar o InterSCity

- Garanta que o **Resource Catalog**, **Data Collector** e **Actuator Controller** do InterSCity estejam ativos.
- Anote a URL base do middleware (ex: `http://<HOST>:<PORT>/api`).

### 4. Inicializar o Servidor

```bash
cd servidor
./backend 8000 ../database.db http://<INTERSCITY_BASE_URL>
```

### 5. Executar o Cliente

```bash
cd cliente
python captura_envio.py --intercity-url http://<INTERSCITY_BASE_URL>
```

### 6. Executar o Dashboard Web

```bash
cd dashboard
python app.py --intercity-url http://<INTERSCITY_BASE_URL>
```

---

## 📝 Funcionalidades

- **Registro de recurso:** Nós de captura se registram no InterSCity Resource Catalog.
- **Publicação de eventos de captura:** Imagens faciais são enviadas como eventos ao Data Collector.
- **Processamento facial:** Backend consome eventos, executa reconhecimento e publica resultados de autorização.
- **Atuação distribuída:** Actuator Controller aciona liberação de acesso conforme evento publicado.
- **Persistência local:** Logs em SQLite para falhas ou auditoria offline.
- **Dashboard:** Interface administrativa consumindo APIs InterSCity para CRUD de usuários e visualização de logs.

---

## 🔒 Segurança

- **TLS 1.3** nas chamadas HTTP ao InterSCity.
- **RBAC** implementado no dashboard via FastAPI.
- **Federated Learning** exploratório para treinamento sem expor dados brutos.

---

## ⚙️ Evoluções Futuras

- Biometria multivariada (face + voz).
- Reconhecimento 3D para maior robustez.
- CI/CD com GitHub Actions e containers Docker.
- Orquestração com Kubernetes.

---

## 👥 Autores

- **Luã Saunders** – Backend em C e integração com InterSCity.
- **Equipe** – Cliente embarcado e dashboard.
---

## 📄 Licença

MIT License
