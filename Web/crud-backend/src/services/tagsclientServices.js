import { query } from "../db.js"

export const getClients = async() => {
    const {rows} = await query('SELECT * FROM tags');
    return rows;
}

export const createClient = async(clientData) => {
    const { tag_id, description, created_at } = clientData;
    const { rows } = await query(
        `INSERT INTO tags (tag_id, description) 
         VALUES ($1, $2) RETURNING *`,
        [tag_id,description]
      );
    
    return rows[0];
}


export const updateClient = async (clientId, clientData) => {
    const { description } = clientData;
  
    const { rows } = await query(
      `UPDATE tags SET  description = $1
       WHERE tag_id = $2 RETURNING *`,
      [ description, clientId]
    );
  
    return rows[0];
  };


export const deleteClient = async (clientId) => {
    const { rowCount } = await query(`DELETE FROM tags WHERE tag_id = $1`, [clientId]);
    return rowCount > 0; // Returns true if a row was deleted, false otherwise
};

export const searchClients = async (searchTerm) => {
    const { rows } = await query(
      `SELECT * FROM tags WHERE tag_id ILIKE $1 OR description ILIKE $1`,
      [`%${searchTerm}%`]
    );
    return rows;
  };