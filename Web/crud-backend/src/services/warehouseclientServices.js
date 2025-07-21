import { query } from "../db.js"

export const getClients = async() => {
    const {rows} = await query('SELECT * FROM warehouse_shelves');
    return rows;
}

export const createClient = async(clientData) => {
    const { name, email, job, rate, isactive } = clientData;
    const { rows } = await query(
        `INSERT INTO clients_tb (shelf_id, shelf_location, warehouse_id, warehouse_name) 
         VALUES ($1, $2, $3, $4) RETURNING *`,
        [shelf_id,shelf_location,warehouse_id,warehouse_name]
      );
    
    return rows[0];
}


export const updateClient = async (clientId, clientData) => {
    const { shelf_id,shelf_location,warehouse_id,warehouse_name } = clientData;
  
    const { rows } = await query(
      `UPDATE warehouse_shelves SET shelf_id = $1, shelf_location = $2, warehouse_id = $3, warehouse_name = $4 
       WHERE id = $5 RETURNING *`,
      [shelf_id, shelf_location, warehouse_id, warehouse_name, clientId]
    );
  
    return rows[0];
  };


export const deleteClient = async (clientId) => {
    const { rowCount } = await query(`DELETE FROM warehouse_shelves WHERE id = $1`, [clientId]);
    return rowCount > 0; // Returns true if a row was deleted, false otherwise
};

export const searchClients = async (searchTerm) => {
    const { rows } = await query(
      `SELECT * FROM warehouse_shelves WHERE shelf_id ILIKE $1 OR shelf_location ILIKE $1 OR warehouse_id ILIKE $1 ILIKE $1 OR warehouse_name`,
      [`%${searchTerm}%`]
    );
    return rows;
  };