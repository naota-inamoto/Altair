
/* ======== process engine table ======== */
drop table ProcDef;
drop table ProcessDefinition;
drop table ProcessDefaultProperties;
drop table ProcessQueue;
drop table CmdQueue;
drop table ProcessProperties;
drop table ProcessActivities;
drop table ProcessLog;
drop sequence ProcessId_seq;
drop sequence CmdQueueId_seq;
drop sequence ProcessLogId_seq;

/* ======== message data table ======== */
drop table TagPathName;
drop table ParentRelation;
drop table MsgData;
drop sequence tagId_seq;
drop sequence msgId_seq;
drop table CommLog;
drop sequence CommLogId_seq;

/* ======== access log table ======== */
drop table AccessLog;
drop sequence AccessLogId_seq;

/* ======== proxy log table ======== */
drop table ProxyLog;
drop sequence ProxyLogId_seq;

/* ======== system log table ======== */
drop table SystemLog;
drop sequence SystemLogId_seq;

