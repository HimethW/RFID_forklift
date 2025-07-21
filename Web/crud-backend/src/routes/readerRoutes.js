import express from 'express';

import * as readerController from '../controllers/readerControllers.js'

const router = express.Router();

router.get('/readers', readerController.getClients);
router.post('/readers', readerController.createClient);
router.put('/readers/:id', readerController.updateClient);
router.delete('/readers/:id', readerController.deleteClient);
router.get('/readers/search', readerController.searchClients); 



export default router;