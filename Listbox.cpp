void CRTUClientDlg::OnItemChangedListip(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here

	if (m_stationSet.IsInited())
	{
		NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
// 		int i=pNMListView->iItem;
// 		char szrow[2];
// 		itoa(1,szrow,10);
// 		AfxMessageBox(szrow);
		
		if (pNMListView->uOldState == 0 && pNMListView->uNewState == 0)
			return;    // No change

		// Old check box state
		BOOL bPrevState = (BOOL)(((pNMListView->uOldState &
			LVIS_STATEIMAGEMASK)>>12)-1); 
		if (bPrevState < 0)    // On startup there's no previous state
			bPrevState = 0; // so assign as false (unchecked)
		
		// New check box state
		BOOL bChecked =
			(BOOL)(((pNMListView->uNewState &LVIS_STATEIMAGEMASK)>>12)-1);  
		if (bChecked < 0) // On non-checkbox notifications assume false
			bChecked = 0;
		
		if (bPrevState == bChecked) // No change in check box
        return;    // Now bChecked holds the new check box state
		
		//pNMListView->iItem ÐÐ
		//pNMListView->iSubItemÁÐ

		//m_ListIP.GetItemText()
		
		if (bChecked)
		{
			//AfxMessageBox("check ");
			m_stationSet.SelectOne(pNMListView->iItem);

		}
		else
		{
			//AfxMessageBox("check ");
			m_stationSet.UnSelectOne(pNMListView->iItem);//,TRUE);
		}
		
	}
	
	
	*pResult = 0;
}

