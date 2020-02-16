
/* ======== process engine table ======== */

create table ProcDef (
    ProcessDefName    varchar2(63)  not null,
    constraint pkey_ProcDef primary key (ProcessDefName)
);

create table ProcessDefinition (
    ProcessDefName    varchar2(63)  not null,
    ActivityId        varchar2(63)  not null,
    ActivityKind      varchar2(63)  not null,
    ActivityName      varchar2(63),
    PropertyValue     varchar2(255) not null, /* for OrSplit */
    AndJoinCount      varchar2(3),   /* for AndJoin */
    Time              varchar2(22),  /* wait time of wait activity */
    NextActivityId    varchar2(63),
    NextActivityKind  varchar2(63),
    NextActivityName  varchar2(63),
    NextPropertyName  varchar2(63),  /* for next OrSpplit */
    ClassName         varchar2(255), /* for Automated */
    constraint pkey_ProcessDef primary key (ProcessDefName, ActivityId, PropertyValue)
);

create table ProcessDefaultProperties (
    ProcessDefName  varchar2(63) not null,
    Name            varchar2(63) not null,
    Value           varchar2(3000),
    constraint pkey_ProcessDefaultProps primary key (ProcessDefName, Name)
);

create table ProcessQueue (
    ProcessDefName         varchar2(63) not null,
    ProcessId              varchar2(63) not null,
    CurrActivityId         varchar2(63),
    CurrActivityKind       varchar2(63),
    CurrActivityName       varchar2(63),
    ActivityId             varchar2(63) not null,
    ActivityKind           varchar2(63) not null,
    ActivityName           varchar2(63),
    PropertyName           varchar2(63),
    Status                 varchar2(10), /* "execute", "wait", "manual", "suspend", "cleanup" */
    Time                   varchar2(22), /* wait time of wait activity */
    constraint  pkey_ProcessQueue primary key (ProcessId, ActivityId)
);

create index idx_ProcessQueue_Status on ProcessQueue ( Status );
create index idx_ProcessQueue_Time   on ProcessQueue ( Time );

create table CmdQueue (
    CmdQueueId    varchar2(63) not null,
    ProcessId     varchar2(63) not null,
    Command       varchar2(20),   /* "putProperty", "suspend", "resume", "abort", "complete" */
    Arg1          varchar2(63),   /* propertyName  ,                              activityId */
    Arg2          varchar2(3000), /* propertyValue */
    constraint  pkey_CmdQueue primary key (CmdQueueId)
);

create table ProcessProperties (
    ProcessId       varchar2(63) not null,
    Name            varchar2(63) not null,
    Value           varchar2(3000),
    constraint pkey_ProcessProps primary key (ProcessId, Name)
);

create table ProcessActivities (
    ProcessDefName    varchar2(63)  not null,
    ProcessId         varchar2(63)  not null,
    ActivityId        varchar2(63)  not null,
    ActivityKind      varchar2(63)  not null,
    ActivityName      varchar2(63),
    PropertyValue     varchar2(255) not null, /* for OrSplit */
    AndJoinCount      varchar2(3),   /* for AndJoin */
    Time              varchar2(23),  /* wait time of wait activity */
    NextActivityId    varchar2(63),
    NextActivityKind  varchar2(63),
    NextActivityName  varchar2(63),
    NextPropertyName  varchar2(63),  /* for next OrSplit */
    ClassName         varchar2(255), /* for Automated */
    Status            varchar2(10),  /* "deactive", "active" */
    Count             varchar2(10),
    constraint pkey_ProcessActivities primary key (ProcessId, ActivityId, PropertyValue)
);

create table ProcessLog (
    ProcessLogId    varchar2(63)  not null,
    Action          varchar2(63),
    Time            varchar2(63)  not null,
    ProcessDefName  varchar2(63),
    ActivityName    varchar2(63),
    ActivityKind    varchar2(63),
    ProcessId       varchar2(63),
    ActivityId      varchar2(63),
    Message         varchar2(3000),
    constraint pkey_ProcessLog primary key (ProcessLogId)
);

create index idx_ProcessLog_Action on ProcessLog ( Action );
create index idx_ProcessLog_Time   on ProcessLog ( Time );

create sequence ProcessId_seq;
create sequence CmdQueueId_seq;
create sequence ProcessLogId_seq;

/* ======== message data table ======== */

create table TagPathName (
    tagId      varchar2(255)   not null,
    path       varchar2(1000),
    msgType    varchar2(255)   not null,
    msgVersion varchar2(255)   not null,
    constraint pkey_TagPathName primary key (tagId)
);
create index idx_TagPathName_msgType  on TagPathName ( msgType );
create index idx_TagPathName_version  on TagPathName ( msgVersion );

create table ParentRelation (
    dataId     varchar2(255)  not null,
    msgId      varchar2(255)  not null,
    msgType    varchar2(255),
    msgVersion varchar2(255),
    dtdFile    varchar2(255),
    parentId   varchar2(255)  not null,
    tagId      varchar2(255)  not null,
    childSeq   varchar2(255)  not null,
    fieldSeq   varchar2(255)  not null,
    constraint pkey_ParentRelation primary key (dataId)
);
create index idx_ParentRelation_msgId    on ParentRelation ( msgId );
create index idx_ParentRelation_parentId on ParentRelation ( parentId );
create index idx_ParentRelation_tagId    on ParentRelation ( tagId );

create table MsgData (
    dataId     varchar2(255)  not null,
    msgId      varchar2(255)  not null,
    parentId   varchar2(255)  not null,
    tagId      varchar2(255)  not null,
    value      varchar2(3000) not null,
    constraint pkey_MsgData primary key (dataId)
);
create index idx_MsgData_msgId    on MsgData ( msgId );
create index idx_MsgData_parentId on MsgData ( parentId );
create index idx_MsgData_tagId    on MsgData ( tagId );
create index idx_MsgData_value    on MsgData ( value );

create sequence tagId_seq;
create sequence msgId_seq;

create table CommLog (
    CommLogId       varchar2(63) not null,
    Target          varchar2(63),
    SendRecv        varchar2(63),
    LogLevel        varchar2(63),
    Time            varchar2(63),
    MsgType         varchar2(255),
    Reason          varchar2(3000),
    URLfromIP       varchar2(255),
    MyId            varchar2(255),
    PartnerId       varchar2(255),
    rawMsgId        varchar2(255),
    MsgId           varchar2(255),
    RefMsgId        varchar2(255),
    TransactionId   varchar2(255),
    Pid             varchar2(255),
    AppPid          varchar2(255),
    constraint pkey_CommLog primary key (CommLogId)
);

create index idx_CommLog_Time on CommLog ( Time );

create sequence CommLogId_seq;

/* ======== access log table ======== */

create table AccessLog (
    AccessLogId     varchar2(63)  not null,
    Time            varchar2(63)  not null,
    FromIP          varchar2(63),
    Path            varchar2(3000),
    ResponseCode    varchar2(63),
    ResponseSize    varchar2(63),
    constraint pkey_AccessLog primary key (AccessLogId)
);

create index idx_AccessLog_Time on AccessLog ( Time );

create sequence AccessLogId_seq;

/* ======== proxy log table ======== */

create table ProxyLog (
    ProxyLogId      varchar2(63)  not null,
    Time            varchar2(63)  not null,
    FromIP          varchar2(63),
    Path            varchar2(3000),
    Reason          varchar2(3000),
    constraint pkey_ProxyLog primary key (ProxyLogId)
);

create index idx_ProxyLog_Time on ProxyLog ( Time );

create sequence ProxyLogId_seq;

/* ======== system log table ======== */

create table SystemLog (
    SystemLogId     varchar2(63)  not null,
    Time         varchar2(63)  not null,
    LogLevel     varchar2(63),
    Message      varchar2(3000),
    constraint pkey_SystemLog primary key (SystemLogId)
);

create index idx_SystemLog_Time on SystemLog ( Time );

create sequence SystemLogId_seq;

