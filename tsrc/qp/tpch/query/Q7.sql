ALTER SESSION SET EXPLAIN PLAN = ON;
ALTER SYSTEM SET TRCLOG_EXPLAIN_GRAPH = 1;

SET TIMING ON;
SET TIMESCALE MILSEC;

SELECT  SUPP_NATION,
        CUST_NATION,
        L_YEAR,
        SUM(VOLUME) AS REVENUE
FROM ( SELECT 
              N1.N_NAME AS SUPP_NATION,
              N2.N_NAME AS CUST_NATION,
              EXTRACT(L_SHIPDATE, 'YEAR') AS L_YEAR,
              L_EXTENDEDPRICE * (1 - L_DISCOUNT) AS VOLUME
       FROM  SUPPLIER,
             LINEITEM,
             ORDERS,
             CUSTOMER,
             NATION N1,
             NATION N2
       WHERE S_SUPPKEY = L_SUPPKEY
         AND O_ORDERKEY = L_ORDERKEY
         AND C_CUSTKEY = O_CUSTKEY
         AND S_NATIONKEY = N1.N_NATIONKEY
         AND C_NATIONKEY = N2.N_NATIONKEY
         AND ( (N1.N_NAME = 'FRANCE' AND N2.N_NAME = 'GERMANY')
                OR (N1.N_NAME = 'GERMANY' AND N2.N_NAME = 'FRANCE')     )
         AND L_SHIPDATE BETWEEN DATE'01-JAN-1995' AND DATE'31-DEC-1996'
     ) SHIPPING
GROUP BY SUPP_NATION,
         CUST_NATION,
         L_YEAR
ORDER BY SUPP_NATION,
         CUST_NATION,
         L_YEAR;

ALTER SYSTEM SET TRCLOG_EXPLAIN_GRAPH = 0;