import express from 'express';

import * as tagController from '../controllers/tagsclientControllers.js'

const router = express.Router();

router.get('/tags', tagController.getClients);
router.post('/tags',tagController.createClient);
router.put('/tags/:id', tagController.updateClient);
router.delete('/tags/:id', tagController.deleteClient);
router.get('/tags/search', tagController.searchClients); 


export default router;