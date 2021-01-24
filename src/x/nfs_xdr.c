/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "nfs.h"

bool_t
xdr_Stat (XDR *xdrs, Stat *objp)
{
	register int32_t *buf;

	 if (!xdr_enum (xdrs, (enum_t *) objp))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_FType (XDR *xdrs, FType *objp)
{
	register int32_t *buf;

	 if (!xdr_enum (xdrs, (enum_t *) objp))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_FHandle (XDR *xdrs, FHandle objp)
{
	register int32_t *buf;

	 if (!xdr_opaque (xdrs, objp, FHSIZE))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_TimeVal (XDR *xdrs, TimeVal *objp)
{
	register int32_t *buf;

	 if (!xdr_u_int (xdrs, &objp->seconds))
		 return FALSE;
	 if (!xdr_u_int (xdrs, &objp->useconds))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_FAttr (XDR *xdrs, FAttr *objp)
{
	register int32_t *buf;


	if (xdrs->x_op == XDR_ENCODE) {
		 if (!xdr_FType (xdrs, &objp->type))
			 return FALSE;
		buf = XDR_INLINE (xdrs, 10 * BYTES_PER_XDR_UNIT);
		if (buf == NULL) {
			 if (!xdr_u_int (xdrs, &objp->mode))
				 return FALSE;
			 if (!xdr_u_int (xdrs, &objp->nlink))
				 return FALSE;
			 if (!xdr_u_int (xdrs, &objp->uid))
				 return FALSE;
			 if (!xdr_u_int (xdrs, &objp->gid))
				 return FALSE;
			 if (!xdr_u_int (xdrs, &objp->size))
				 return FALSE;
			 if (!xdr_u_int (xdrs, &objp->blocksize))
				 return FALSE;
			 if (!xdr_u_int (xdrs, &objp->rdev))
				 return FALSE;
			 if (!xdr_u_int (xdrs, &objp->blocks))
				 return FALSE;
			 if (!xdr_u_int (xdrs, &objp->fsid))
				 return FALSE;
			 if (!xdr_u_int (xdrs, &objp->fileid))
				 return FALSE;

		} else {
		IXDR_PUT_U_LONG(buf, objp->mode);
		IXDR_PUT_U_LONG(buf, objp->nlink);
		IXDR_PUT_U_LONG(buf, objp->uid);
		IXDR_PUT_U_LONG(buf, objp->gid);
		IXDR_PUT_U_LONG(buf, objp->size);
		IXDR_PUT_U_LONG(buf, objp->blocksize);
		IXDR_PUT_U_LONG(buf, objp->rdev);
		IXDR_PUT_U_LONG(buf, objp->blocks);
		IXDR_PUT_U_LONG(buf, objp->fsid);
		IXDR_PUT_U_LONG(buf, objp->fileid);
		}
		 if (!xdr_TimeVal (xdrs, &objp->atime))
			 return FALSE;
		 if (!xdr_TimeVal (xdrs, &objp->mtime))
			 return FALSE;
		 if (!xdr_TimeVal (xdrs, &objp->ctime))
			 return FALSE;
		return TRUE;
	} else if (xdrs->x_op == XDR_DECODE) {
		 if (!xdr_FType (xdrs, &objp->type))
			 return FALSE;
		buf = XDR_INLINE (xdrs, 10 * BYTES_PER_XDR_UNIT);
		if (buf == NULL) {
			 if (!xdr_u_int (xdrs, &objp->mode))
				 return FALSE;
			 if (!xdr_u_int (xdrs, &objp->nlink))
				 return FALSE;
			 if (!xdr_u_int (xdrs, &objp->uid))
				 return FALSE;
			 if (!xdr_u_int (xdrs, &objp->gid))
				 return FALSE;
			 if (!xdr_u_int (xdrs, &objp->size))
				 return FALSE;
			 if (!xdr_u_int (xdrs, &objp->blocksize))
				 return FALSE;
			 if (!xdr_u_int (xdrs, &objp->rdev))
				 return FALSE;
			 if (!xdr_u_int (xdrs, &objp->blocks))
				 return FALSE;
			 if (!xdr_u_int (xdrs, &objp->fsid))
				 return FALSE;
			 if (!xdr_u_int (xdrs, &objp->fileid))
				 return FALSE;

		} else {
		objp->mode = IXDR_GET_U_LONG(buf);
		objp->nlink = IXDR_GET_U_LONG(buf);
		objp->uid = IXDR_GET_U_LONG(buf);
		objp->gid = IXDR_GET_U_LONG(buf);
		objp->size = IXDR_GET_U_LONG(buf);
		objp->blocksize = IXDR_GET_U_LONG(buf);
		objp->rdev = IXDR_GET_U_LONG(buf);
		objp->blocks = IXDR_GET_U_LONG(buf);
		objp->fsid = IXDR_GET_U_LONG(buf);
		objp->fileid = IXDR_GET_U_LONG(buf);
		}
		 if (!xdr_TimeVal (xdrs, &objp->atime))
			 return FALSE;
		 if (!xdr_TimeVal (xdrs, &objp->mtime))
			 return FALSE;
		 if (!xdr_TimeVal (xdrs, &objp->ctime))
			 return FALSE;
	 return TRUE;
	}

	 if (!xdr_FType (xdrs, &objp->type))
		 return FALSE;
	 if (!xdr_u_int (xdrs, &objp->mode))
		 return FALSE;
	 if (!xdr_u_int (xdrs, &objp->nlink))
		 return FALSE;
	 if (!xdr_u_int (xdrs, &objp->uid))
		 return FALSE;
	 if (!xdr_u_int (xdrs, &objp->gid))
		 return FALSE;
	 if (!xdr_u_int (xdrs, &objp->size))
		 return FALSE;
	 if (!xdr_u_int (xdrs, &objp->blocksize))
		 return FALSE;
	 if (!xdr_u_int (xdrs, &objp->rdev))
		 return FALSE;
	 if (!xdr_u_int (xdrs, &objp->blocks))
		 return FALSE;
	 if (!xdr_u_int (xdrs, &objp->fsid))
		 return FALSE;
	 if (!xdr_u_int (xdrs, &objp->fileid))
		 return FALSE;
	 if (!xdr_TimeVal (xdrs, &objp->atime))
		 return FALSE;
	 if (!xdr_TimeVal (xdrs, &objp->mtime))
		 return FALSE;
	 if (!xdr_TimeVal (xdrs, &objp->ctime))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_SAttr (XDR *xdrs, SAttr *objp)
{
	register int32_t *buf;


	if (xdrs->x_op == XDR_ENCODE) {
		buf = XDR_INLINE (xdrs, 4 * BYTES_PER_XDR_UNIT);
		if (buf == NULL) {
			 if (!xdr_u_int (xdrs, &objp->mode))
				 return FALSE;
			 if (!xdr_u_int (xdrs, &objp->uid))
				 return FALSE;
			 if (!xdr_u_int (xdrs, &objp->gid))
				 return FALSE;
			 if (!xdr_u_int (xdrs, &objp->size))
				 return FALSE;

		} else {
		IXDR_PUT_U_LONG(buf, objp->mode);
		IXDR_PUT_U_LONG(buf, objp->uid);
		IXDR_PUT_U_LONG(buf, objp->gid);
		IXDR_PUT_U_LONG(buf, objp->size);
		}
		 if (!xdr_TimeVal (xdrs, &objp->atime))
			 return FALSE;
		 if (!xdr_TimeVal (xdrs, &objp->mtime))
			 return FALSE;
		return TRUE;
	} else if (xdrs->x_op == XDR_DECODE) {
		buf = XDR_INLINE (xdrs, 4 * BYTES_PER_XDR_UNIT);
		if (buf == NULL) {
			 if (!xdr_u_int (xdrs, &objp->mode))
				 return FALSE;
			 if (!xdr_u_int (xdrs, &objp->uid))
				 return FALSE;
			 if (!xdr_u_int (xdrs, &objp->gid))
				 return FALSE;
			 if (!xdr_u_int (xdrs, &objp->size))
				 return FALSE;

		} else {
		objp->mode = IXDR_GET_U_LONG(buf);
		objp->uid = IXDR_GET_U_LONG(buf);
		objp->gid = IXDR_GET_U_LONG(buf);
		objp->size = IXDR_GET_U_LONG(buf);
		}
		 if (!xdr_TimeVal (xdrs, &objp->atime))
			 return FALSE;
		 if (!xdr_TimeVal (xdrs, &objp->mtime))
			 return FALSE;
	 return TRUE;
	}

	 if (!xdr_u_int (xdrs, &objp->mode))
		 return FALSE;
	 if (!xdr_u_int (xdrs, &objp->uid))
		 return FALSE;
	 if (!xdr_u_int (xdrs, &objp->gid))
		 return FALSE;
	 if (!xdr_u_int (xdrs, &objp->size))
		 return FALSE;
	 if (!xdr_TimeVal (xdrs, &objp->atime))
		 return FALSE;
	 if (!xdr_TimeVal (xdrs, &objp->mtime))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_Filename (XDR *xdrs, Filename *objp)
{
	register int32_t *buf;

	 if (!xdr_bytes (xdrs, (char **)&objp->Filename_val, (u_int *) &objp->Filename_len, MAXNAMLEN))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_Path (XDR *xdrs, Path *objp)
{
	register int32_t *buf;

	 if (!xdr_bytes (xdrs, (char **)&objp->Path_val, (u_int *) &objp->Path_len, NFSMAXPATHLEN))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_NFSData (XDR *xdrs, NFSData *objp)
{
	register int32_t *buf;

	 if (!xdr_bytes (xdrs, (char **)&objp->NFSData_val, (u_int *) &objp->NFSData_len, MAXDATA))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_NFSCookie (XDR *xdrs, NFSCookie objp)
{
	register int32_t *buf;

	 if (!xdr_opaque (xdrs, objp, COOKIESIZE))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_AttrStat (XDR *xdrs, AttrStat *objp)
{
	register int32_t *buf;

	 if (!xdr_Stat (xdrs, &objp->status))
		 return FALSE;
	switch (objp->status) {
	case NFS_OK:
		 if (!xdr_FAttr (xdrs, &objp->AttrStat_u.attributes))
			 return FALSE;
		break;
	default:
		break;
	}
	return TRUE;
}

bool_t
xdr_DirOpArgs (XDR *xdrs, DirOpArgs *objp)
{
	register int32_t *buf;

	 if (!xdr_FHandle (xdrs, objp->dir))
		 return FALSE;
	 if (!xdr_Filename (xdrs, &objp->name))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_DirOpResBody (XDR *xdrs, DirOpResBody *objp)
{
	register int32_t *buf;

	 if (!xdr_FHandle (xdrs, objp->file))
		 return FALSE;
	 if (!xdr_FAttr (xdrs, &objp->attributes))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_DirOpRes (XDR *xdrs, DirOpRes *objp)
{
	register int32_t *buf;

	 if (!xdr_Stat (xdrs, &objp->status))
		 return FALSE;
	switch (objp->status) {
	case NFS_OK:
		 if (!xdr_DirOpResBody (xdrs, &objp->DirOpRes_u.diropok))
			 return FALSE;
		break;
	default:
		break;
	}
	return TRUE;
}

bool_t
xdr_SAttrArgs (XDR *xdrs, SAttrArgs *objp)
{
	register int32_t *buf;

	 if (!xdr_FHandle (xdrs, objp->file))
		 return FALSE;
	 if (!xdr_SAttr (xdrs, &objp->attributes))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_ReadLinkRes (XDR *xdrs, ReadLinkRes *objp)
{
	register int32_t *buf;

	 if (!xdr_Stat (xdrs, &objp->status))
		 return FALSE;
	switch (objp->status) {
	case NFS_OK:
		 if (!xdr_Path (xdrs, &objp->ReadLinkRes_u.data))
			 return FALSE;
		break;
	default:
		break;
	}
	return TRUE;
}

bool_t
xdr_ReadArgs (XDR *xdrs, ReadArgs *objp)
{
	register int32_t *buf;

	 if (!xdr_FHandle (xdrs, objp->file))
		 return FALSE;
	 if (!xdr_u_int (xdrs, &objp->offset))
		 return FALSE;
	 if (!xdr_u_int (xdrs, &objp->count))
		 return FALSE;
	 if (!xdr_u_int (xdrs, &objp->totalcount))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_ReadResBody (XDR *xdrs, ReadResBody *objp)
{
	register int32_t *buf;

	 if (!xdr_FAttr (xdrs, &objp->attributes))
		 return FALSE;
	 if (!xdr_NFSData (xdrs, &objp->data))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_ReadRes (XDR *xdrs, ReadRes *objp)
{
	register int32_t *buf;

	 if (!xdr_Stat (xdrs, &objp->status))
		 return FALSE;
	switch (objp->status) {
	case NFS_OK:
		 if (!xdr_ReadResBody (xdrs, &objp->ReadRes_u.readResOk))
			 return FALSE;
		break;
	default:
		break;
	}
	return TRUE;
}

bool_t
xdr_WriteArgs (XDR *xdrs, WriteArgs *objp)
{
	register int32_t *buf;


	if (xdrs->x_op == XDR_ENCODE) {
		 if (!xdr_FHandle (xdrs, objp->file))
			 return FALSE;
		buf = XDR_INLINE (xdrs, 3 * BYTES_PER_XDR_UNIT);
		if (buf == NULL) {
			 if (!xdr_u_int (xdrs, &objp->beginOffset))
				 return FALSE;
			 if (!xdr_u_int (xdrs, &objp->offset))
				 return FALSE;
			 if (!xdr_u_int (xdrs, &objp->totalCount))
				 return FALSE;

		} else {
		IXDR_PUT_U_LONG(buf, objp->beginOffset);
		IXDR_PUT_U_LONG(buf, objp->offset);
		IXDR_PUT_U_LONG(buf, objp->totalCount);
		}
		 if (!xdr_NFSData (xdrs, &objp->data))
			 return FALSE;
		return TRUE;
	} else if (xdrs->x_op == XDR_DECODE) {
		 if (!xdr_FHandle (xdrs, objp->file))
			 return FALSE;
		buf = XDR_INLINE (xdrs, 3 * BYTES_PER_XDR_UNIT);
		if (buf == NULL) {
			 if (!xdr_u_int (xdrs, &objp->beginOffset))
				 return FALSE;
			 if (!xdr_u_int (xdrs, &objp->offset))
				 return FALSE;
			 if (!xdr_u_int (xdrs, &objp->totalCount))
				 return FALSE;

		} else {
		objp->beginOffset = IXDR_GET_U_LONG(buf);
		objp->offset = IXDR_GET_U_LONG(buf);
		objp->totalCount = IXDR_GET_U_LONG(buf);
		}
		 if (!xdr_NFSData (xdrs, &objp->data))
			 return FALSE;
	 return TRUE;
	}

	 if (!xdr_FHandle (xdrs, objp->file))
		 return FALSE;
	 if (!xdr_u_int (xdrs, &objp->beginOffset))
		 return FALSE;
	 if (!xdr_u_int (xdrs, &objp->offset))
		 return FALSE;
	 if (!xdr_u_int (xdrs, &objp->totalCount))
		 return FALSE;
	 if (!xdr_NFSData (xdrs, &objp->data))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_CreateArgs (XDR *xdrs, CreateArgs *objp)
{
	register int32_t *buf;

	 if (!xdr_DirOpArgs (xdrs, &objp->where))
		 return FALSE;
	 if (!xdr_SAttr (xdrs, &objp->attributes))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_RenameArgs (XDR *xdrs, RenameArgs *objp)
{
	register int32_t *buf;

	 if (!xdr_DirOpArgs (xdrs, &objp->from))
		 return FALSE;
	 if (!xdr_DirOpArgs (xdrs, &objp->to))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_LinkArgs (XDR *xdrs, LinkArgs *objp)
{
	register int32_t *buf;

	 if (!xdr_FHandle (xdrs, objp->from))
		 return FALSE;
	 if (!xdr_DirOpArgs (xdrs, &objp->to))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_SymLinkArgs (XDR *xdrs, SymLinkArgs *objp)
{
	register int32_t *buf;

	 if (!xdr_DirOpArgs (xdrs, &objp->from))
		 return FALSE;
	 if (!xdr_Path (xdrs, &objp->to))
		 return FALSE;
	 if (!xdr_SAttr (xdrs, &objp->attributes))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_ReadDirArgs (XDR *xdrs, ReadDirArgs *objp)
{
	register int32_t *buf;

	 if (!xdr_FHandle (xdrs, objp->dir))
		 return FALSE;
	 if (!xdr_NFSCookie (xdrs, objp->cookie))
		 return FALSE;
	 if (!xdr_u_int (xdrs, &objp->count))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_Entry (XDR *xdrs, Entry *objp)
{
	register int32_t *buf;

	 if (!xdr_u_int (xdrs, &objp->fileId))
		 return FALSE;
	 if (!xdr_Filename (xdrs, &objp->name))
		 return FALSE;
	 if (!xdr_NFSCookie (xdrs, objp->cookie))
		 return FALSE;
	 if (!xdr_pointer (xdrs, (char **)&objp->next, sizeof (Entry), (xdrproc_t) xdr_Entry))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_ReadDirResBody (XDR *xdrs, ReadDirResBody *objp)
{
	register int32_t *buf;

	 if (!xdr_pointer (xdrs, (char **)&objp->entries, sizeof (Entry), (xdrproc_t) xdr_Entry))
		 return FALSE;
	 if (!xdr_bool (xdrs, &objp->eof))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_ReadDirRes (XDR *xdrs, ReadDirRes *objp)
{
	register int32_t *buf;

	 if (!xdr_Stat (xdrs, &objp->status))
		 return FALSE;
	switch (objp->status) {
	case NFS_OK:
		 if (!xdr_ReadDirResBody (xdrs, &objp->ReadDirRes_u.readdirok))
			 return FALSE;
		break;
	default:
		break;
	}
	return TRUE;
}

bool_t
xdr_StatFSResBody (XDR *xdrs, StatFSResBody *objp)
{
	register int32_t *buf;


	if (xdrs->x_op == XDR_ENCODE) {
		buf = XDR_INLINE (xdrs, 5 * BYTES_PER_XDR_UNIT);
		if (buf == NULL) {
			 if (!xdr_u_int (xdrs, &objp->tsize))
				 return FALSE;
			 if (!xdr_u_int (xdrs, &objp->bsize))
				 return FALSE;
			 if (!xdr_u_int (xdrs, &objp->blocks))
				 return FALSE;
			 if (!xdr_u_int (xdrs, &objp->bfree))
				 return FALSE;
			 if (!xdr_u_int (xdrs, &objp->bavail))
				 return FALSE;
		} else {
			IXDR_PUT_U_LONG(buf, objp->tsize);
			IXDR_PUT_U_LONG(buf, objp->bsize);
			IXDR_PUT_U_LONG(buf, objp->blocks);
			IXDR_PUT_U_LONG(buf, objp->bfree);
			IXDR_PUT_U_LONG(buf, objp->bavail);
		}
		return TRUE;
	} else if (xdrs->x_op == XDR_DECODE) {
		buf = XDR_INLINE (xdrs, 5 * BYTES_PER_XDR_UNIT);
		if (buf == NULL) {
			 if (!xdr_u_int (xdrs, &objp->tsize))
				 return FALSE;
			 if (!xdr_u_int (xdrs, &objp->bsize))
				 return FALSE;
			 if (!xdr_u_int (xdrs, &objp->blocks))
				 return FALSE;
			 if (!xdr_u_int (xdrs, &objp->bfree))
				 return FALSE;
			 if (!xdr_u_int (xdrs, &objp->bavail))
				 return FALSE;
		} else {
			objp->tsize = IXDR_GET_U_LONG(buf);
			objp->bsize = IXDR_GET_U_LONG(buf);
			objp->blocks = IXDR_GET_U_LONG(buf);
			objp->bfree = IXDR_GET_U_LONG(buf);
			objp->bavail = IXDR_GET_U_LONG(buf);
		}
	 return TRUE;
	}

	 if (!xdr_u_int (xdrs, &objp->tsize))
		 return FALSE;
	 if (!xdr_u_int (xdrs, &objp->bsize))
		 return FALSE;
	 if (!xdr_u_int (xdrs, &objp->blocks))
		 return FALSE;
	 if (!xdr_u_int (xdrs, &objp->bfree))
		 return FALSE;
	 if (!xdr_u_int (xdrs, &objp->bavail))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_StatFSRes (XDR *xdrs, StatFSRes *objp)
{
	register int32_t *buf;

	 if (!xdr_Stat (xdrs, &objp->status))
		 return FALSE;
	switch (objp->status) {
	case NFS_OK:
		 if (!xdr_StatFSResBody (xdrs, &objp->StatFSRes_u.info))
			 return FALSE;
		break;
	default:
		break;
	}
	return TRUE;
}
