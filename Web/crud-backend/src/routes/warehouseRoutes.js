import express from 'express';

import * as warehouseController from '../controllers/warehouseclientControllers.js'

const router = express.Router();

router.get('/warehouse', warehouseController.getClients);
router.post('/warehouse', warehouseController.createClient);
router.put('/warehouse/:id', warehouseController.updateClient);
router.delete('/warehouse/:id', warehouseController.deleteClient);
router.get('/warehouse/search', warehouseController.searchClients); 



export default router;