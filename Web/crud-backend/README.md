# RFID Scanning and Inventory System Backend 

A Node.js Express-based REST API backend for managing clients, tags, warehouses, readers, and scanning operations with real-time WebSocket communication.

## 🚀 Features

- **CRUD Operations**: Complete Create, Read, Update, Delete operations for multiple entities
- **Authentication**: JWT-based user authentication system
- **Real-time Communication**: WebSocket server for live scan data broadcasting
- **ESP32 Integration**: Direct endpoints for ESP32 device communication
- **PostgreSQL Database**: Robust data persistence with PostgreSQL
- **RESTful API**: Well-structured REST endpoints
- **CORS Support**: Cross-origin resource sharing enabled

## 📋 API Endpoints

### Authentication
- `POST /api/login` - User authentication

### Clients
- `GET /api/clients` - Get all clients
- `POST /api/clients` - Create new client
- `PUT /api/clients/:id` - Update client
- `DELETE /api/clients/:id` - Delete client
- `GET /api/clients/search` - Search clients

### Tag Clients
- CRUD operations for tag-client relationships

### Warehouses
- CRUD operations for warehouse management

### Readers
- CRUD operations for RFID/scanning device readers

### Users
- User management operations

### Scans
- Scan data management and tracking

### ESP32 Integration
- `POST /api/esp/send-id` - Receive scan data from ESP32 devices

## 🛠️ Tech Stack

- **Runtime**: Node.js
- **Framework**: Express.js
- **Database**: PostgreSQL
- **Authentication**: JWT (JSON Web Tokens)
- **Password Hashing**: bcrypt
- **WebSocket**: ws library
- **Environment Variables**: dotenv
- **CORS**: cors middleware

## 📦 Installation

1. **Clone the repository**
   ```bash
   git clone <repository-url>
   cd crud-backend
   ```

2. **Install dependencies**
   ```bash
   npm install
   ```

3. **Set up environment variables**
   
   Create a `.env` file in the root directory with the following variables:
   ```env
   PG_USER=your_postgres_username
   PG_PASSWORD=your_postgres_password
   PG_HOST=your_postgres_host
   PG_DATABASE=your_database_name
   PG_PORT=5432
   JWT_SECRET=your_jwt_secret_key
   ```

4. **Set up PostgreSQL Database**
   
   Make sure you have PostgreSQL installed and create the necessary database and tables.

## 🚀 Running the Application

### Development Mode
```bash
npm run dev
```
This uses nodemon for automatic restarts on file changes.

### Production Mode
```bash
npm start
```

The server will start on port 80 by default.

## 🌐 Deployment

This project is configured for deployment on Vercel with the included `vercel.json` configuration file.

### Deploy to Vercel
1. Install Vercel CLI: `npm i -g vercel`
2. Run: `vercel`
3. Follow the deployment prompts

## 📡 WebSocket Communication

The server includes a WebSocket server that broadcasts real-time scan data to connected clients. 

**WebSocket Endpoint**: `ws://localhost:80` (or your deployed URL)

When a new scan is received from an ESP32 device, it's automatically broadcasted to all connected WebSocket clients.

## 🔧 Project Structure

```
src/
├── controllers/          # Request handlers
│   ├── authController.js
│   ├── clientController.js
│   ├── readerControllers.js
│   ├── scanControllers.js
│   ├── tagsclientControllers.js
│   ├── userControllers.js
│   └── warehouseclientControllers.js
├── routes/              # API route definitions
│   ├── authRoutes.js
│   ├── clientRoutes.js
│   ├── espRoutes.js
│   ├── readerRoutes.js
│   ├── scanRoutes.js
│   ├── tagsclientRoutes.js
│   ├── userRoutes.js
│   └── warehouseRoutes.js
├── services/            # Business logic layer
│   ├── authServices.js
│   ├── clientServices.js
│   ├── readerSevices.js
│   ├── tagsclientServices.js
│   ├── usersServices.js
│   └── warehouseclientServices.js
├── db.js               # Database connection
└── index.js            # Main application entry point
```

## 🔐 Authentication

The API uses JWT (JSON Web Tokens) for authentication. Include the token in the Authorization header:

```
Authorization: Bearer <your-jwt-token>
```

## 📊 Database Schema

The application uses PostgreSQL with the following main entities:
- **Users**: User accounts and authentication
- **Clients**: Customer/client information
- **Tags**: RFID tag data
- **Warehouses**: Warehouse locations
- **Readers**: RFID reader devices
- **Scans**: Scan event records

## 🤝 Contributing

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/new-feature`)
3. Commit your changes (`git commit -am 'Add new feature'`)
4. Push to the branch (`git push origin feature/new-feature`)
5. Create a Pull Request

## 📝 License

This project is licensed under the ISC License.

## 🐛 Troubleshooting

### Common Issues

1. **Database Connection Issues**
   - Verify PostgreSQL is running
   - Check environment variables in `.env`
   - Ensure database exists and is accessible

2. **Port Already in Use**
   - The application runs on port 80 by default
   - Make sure no other application is using this port
   - Run with administrator privileges if needed

3. **JWT Authentication Errors**
   - Ensure `JWT_SECRET` is set in environment variables
   - Check token expiration (tokens expire after 1 hour)

## 📞 Support

For support and questions, please create an issue in the repository.
