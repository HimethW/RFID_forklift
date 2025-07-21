import { query } from "../db.js"

export const getClients = async() => {
    const {rows} = await query('SELECT * FROM Readers');
    return rows;
}

export const createClient = async(clientData) => {
    const { reader_id, reader_name, reader_type, status, last_picked_up } = clientData;
    const { rows } = await query(
        `INSERT INTO Readers (reader_id, reader_name, reader_type, status, last_picked_up) 
         VALUES ($1, $2, $3, $4, $5) RETURNING *`,
        [reader_id, reader_name, reader_type, status, last_picked_up]
      );
    
    return rows[0];
}


export const updateClient = async (clientId, clientData) => {
    const { reader_id, reader_name, reader_type, status, last_picked_up } = clientData;
  
    const { rows } = await query(
      `UPDATE Readers SET reader_id = $1, reader_name = $2, reader_type = $3, status = $4, last_picked_up = $5 
       WHERE id = $6 RETURNING *`,
      [reader_id, reader_name, reader_type, status, last_picked_up, clientId]
    );
  
    return rows[0];
  };


export const deleteClient = async (clientId) => {
    const { rowCount } = await query(`DELETE FROM Readers WHEREid = $1`, [clientId]);
    return rowCount > 0; // Returns true if a row was deleted, false otherwise
};

export const searchClients = async (searchTerm) => {
    const { rows } = await query(
      `SELECT * FROM Readers WHERE reader_id ILIKE $1 OR reader_name ILIKE $1 OR reader_type ILIKE $1`,
      [`%${searchTerm}%`]
    );
    return rows;
  };