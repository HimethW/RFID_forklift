import { useState, useEffect } from "react"

export default function ModalFormTags({ isOpen , onClose, mode, OnSubmit,clientData}) {
    const [tag_id, settagID] = useState('');
    const [description, setDescription] = useState('');

    // Handle the change of status


    const handleSubmit = async (e) => {
        e.preventDefault();
        try {
            const clientData = {tag_id, description}
            console.log("Submitting data:", {
              tag_id,
              description
            });

            await OnSubmit(clientData)
            onClose();
        } catch (err) {
            console.error("Error adding tag" , err);
        }
        
    }

    useEffect(() => {
        if (mode === 'edit' && clientData) {
            settagID(clientData.tag_id);
            setDescription(clientData.description);
        } else {
            // Reset fields when adding a new client
            settagID('');
            setDescription('');

        }
    }, [mode, clientData]);


    return (



        
        <>
            {/* You can open the modal using document.getElementById('ID').showModal() method */}
            <dialog id="my_modal_3" className="modal" open={isOpen}>
            <div className="modal-box">
                <h3 className="font-bold text-lg py-4">{mode === 'edit' ? 'Edit Tag' : 'Tag Details' }</h3>
                <form method="dialog" onSubmit={handleSubmit} >
                {/* if there is a button in form, it will close the modal */}

                <label className="input input-bordered my-4 flex items-center gap-2">
                    TagID
                    <input type="text" className="grow" value={tag_id} onChange={(e) => settagID(e.target.value)}/>
                </label>
                <label className="input input-bordered my-4 flex items-center gap-2">
                    Description
                    <input type="text" className="grow" value={description} onChange={(e) => setDescription(e.target.value)} />
                </label>
  

     


                <button type="button" className="btn btn-sm btn-circle btn-ghost absolute right-2 top-2"  onClick={onClose}>✕</button>
                
                <button type="submit" className="btn btn-success"> {mode === 'edit' ? 'Save Changes' : 'Add Tag' }</button>
                </form>
            </div>
            </dialog>
        </>
    )
}