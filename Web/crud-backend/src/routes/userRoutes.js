import express from 'express';

import * as userController from '../controllers/userControllers.js'

const router = express.Router();

router.get('/users', userController.getClients);
router.post('/users', userController.createClient);
router.put('/users/:id', userController.updateClient);
router.delete('/users/:id', userController.deleteClient);
router.get('/users/search', userController.searchClients); 



export default router;