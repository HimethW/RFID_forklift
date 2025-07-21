
# RFID Scanning and Inventory System Frontend

This is a frontend web application for managing CRUD (Create, Read, Update, Delete) operations, built with React, Vite, and Tailwind CSS.

## Features
- User authentication (Login)
- Manage Clients, Readers, Tags, Users, and Warehouse
- Modal forms for adding and editing records
- Responsive design with Tailwind CSS
- Navigation bar for easy access to different modules

## Project Structure
```
├── public/
├── src/
│   ├── assets/
│   ├── components/
│   ├── pages/
│   ├── routes/
│   ├── App.css
│   ├── main.jsx
│   └── ...
├── index.html
├── package.json
├── tailwind.config.js
├── vite.config.js
└── ...
```

## Getting Started

### Prerequisites
- Node.js (v16 or higher recommended)
- npm or yarn

### Installation
1. Clone the repository:
   ```sh
   git clone <repository-url>
   cd crud-frontend
   ```
2. Install dependencies:
   ```sh
   npm install
   # or
   yarn install
   ```

### Running the App
Start the development server:
```sh
npm run dev
# or
yarn dev
```
The app will be available at `http://localhost:5173` by default.

### Building for Production
To build the app for production:
```sh
npm run build
# or
yarn build
```

### Linting
To lint the codebase:
```sh
npm run lint
# or
yarn lint
```

## Technologies Used
- [React](https://react.dev/)
- [Vite](https://vitejs.dev/)
- [Tailwind CSS](https://tailwindcss.com/)
- [ESLint](https://eslint.org/)

## License
This project is licensed under the MIT License.
